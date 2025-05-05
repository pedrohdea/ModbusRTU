#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600
#define TIMEOUT_INTERBYTE_MS 4 // 3.5 bytes @ 9600bps ≈ 4ms

/**
 * @brief Envia uma requisição Modbus RTU para escrever múltiplos coils (função 0x0F).
 *
 * Essa função monta e envia um frame Modbus RTU para escrever `quantidade` coils
 * a partir do endereço `inicio`, utilizando os dados fornecidos no buffer `dados`.
 * A comunicação é feita via porta serial previamente configurada.
 *
 * @param inicio Índice inicial do primeiro coil (0 a 65535). Representa o endereço Modbus.
 * @param quantidade Número total de coils a serem escritos (1 a 64).
 * @param byteCount Número de bytes necessários para armazenar os `quantidade` bits (coils).
 * @param dados Vetor de bytes contendo os valores dos coils a serem escritos.
 *             Os bits devem estar organizados em formato LSB-first, como exige o protocolo Modbus.
 *
 * @note A função realiza abertura e fechamento da porta serial.
 *       Verifica a resposta do escravo e valida o CRC de retorno.
 *
 * @warning Essa função suporta no máximo 64 coils por requisição (8 bytes).
 *
 * @example
 * unsigned char dados[8] = {0xFF, 0x00}; // Ativa os primeiros 8 coils
 * escreverMultiplosCoils(0x0010, 8, 1, dados); // Escreve 8 coils a partir do endereço 0x0010
 */
int escreverMultiplosCoils(int inicio, int quantidade, int byteCount, const unsigned char *dados)
{
    unsigned char req[260] = {0};
    req[0] = 0x01; // ID Escravo
    req[1] = 0x0F; // Função
    req[2] = (inicio >> 8) & 0xFF;
    req[3] = inicio & 0xFF;
    req[4] = (quantidade >> 8) & 0xFF;
    req[5] = quantidade & 0xFF;
    req[6] = byteCount;

    memcpy(&req[7], dados, byteCount);

    unsigned short crc = CRC16((char *)req, 7 + byteCount);
    req[7 + byteCount] = crc & 0xFF;
    req[8 + byteCount] = crc >> 8;

    int total = 9 + byteCount;

    if (!serialOpen(PORTA_COM, BAUDRATE, 8, NOPARITY, TWOSTOPBITS))
    {
        fprintf(stderr, "❌ Erro ao abrir porta serial.\n");
        return 0;
    }

    printf("📤 Enviando: ");
    exibeDados((const char *)req, total);
    serialWrite((const char *)req, total);

    char *resposta = lerResposta();
    if (resposta)
    {
        printf("✅ Resposta válida recebida.\n");
    }
    else
    {
        printf("❌ Nenhuma resposta válida.\n");
    }

    serialClose();
    return 1;
}

int posicaoParaIndice(char coluna, char linha)
{
    coluna = toupper(coluna);
    if (coluna < 'A' || coluna > 'H' || linha < '1' || linha > '8')
        return -1;
    int col = coluna - 'A';
    int lin = linha - '1';
    return lin * 8 + col;
}

void sendPosicao(void)
{
    // 🔍 Conversão da posição:
    // A2 → linha 2, coluna A → índice: (2 - 1) * 8 + ('A' - 'A') = 8
    // H8 → linha 8, coluna H → índice: (8 - 1) * 8 + ('H' - 'A') = 63
    // ✅ Parâmetros resultantes:
    // inicio = 8
    // fim = 63
    // quantidade = fim - inicio + 1 = 56
    // byteCount = (quantidade + 7) / 8 = (56 + 7) / 8 = 63 / 8 = 7 (arredondado para cima)

    char comando[5];
    printf("Digite a jogada (ex: A2H8): ");
    scanf("%4s", comando);

    if (strlen(comando) != 4)
    {
        printf("❌ Entrada inválida. Use 4 caracteres (ex: A2H8).\n");
        return;
    }

    printf("📍 Início: coluna = %c, linha = %c\n", comando[0], comando[1]);
    printf("📍 Fim:    coluna = %c, linha = %c\n", comando[2], comando[3]);

    int inicio = posicaoParaIndice(comando[0], comando[1]);
    int fim = posicaoParaIndice(comando[2], comando[3]);

    if (inicio == -1 || fim == -1 || inicio > fim)
    {
        printf("❌ Jogada inválida.\n");
        return;
    }

    int quantidade = fim - inicio + 1;
    int byteCount = (quantidade + 7) / 8;

    if (quantidade > 64)
    {
        printf("❌ Máximo de 64 coils por requisição.\n");
        return;
    }

    unsigned char dados[8] = {0};
    for (int i = 0; i < quantidade; i++)
    {
        dados[i / 8] |= (1 << (i % 8)); // LSB-first (padrão Modbus)
    };
    escreverMultiplosCoils(inicio, quantidade, byteCount, dados);
}
