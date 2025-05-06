#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mapa.h"
#include <unistd.h>

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

    modbusWrite((const char *)req, total);

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

/**
 * @brief Lê uma jogada do usuário no formato "A2H8" e aciona os coils correspondentes no tabuleiro.
 *
 * Essa função solicita ao usuário que digite uma jogada no formato de coordenadas iniciais e finais
 * (por exemplo, "A2H8"), converte essas posições para um mapa de bits (coils) e envia uma requisição
 * Modbus RTU para ativar os LEDs correspondentes usando a função 0x0F (Write Multiple Coils).
 *
 * A jogada é mapeada em um vetor de 16 bytes, mas apenas os dois primeiros bytes são utilizados (16 bits),
 * sendo cada bit responsável por acionar uma posição na matriz 8x8 do tabuleiro.
 *
 * Exemplo:
 * - Jogada: "A2H8"
 * - Resultado esperado nos coils:
 *   - Byte 0: colunas ativadas (bit 0 para 'A', bit 7 para 'H')
 *   - Byte 1: linhas ativadas (bit 1 para '2', bit 7 para '8')
 *
 * @note Essa função assume que a matriz do tabuleiro tem exatamente 8 colunas e 8 linhas (64 posições),
 *       e que o hardware usa dois 74HC595 para controlar 16 LEDs/coils.
 */
void sendPosicao(void)
{
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

    uint8_t posicaoDe[16] = {0};
    uint8_t posicaoPara[16] = {0};
    gerarMapaCoils(comando, posicaoDe, posicaoPara);  // Preenche os 2 primeiros bytes

    printf("🧠 Coils a enviar:\n");
    exibeBits(posicaoDe, 2);  // Mostra só os dois bytes
    exibeBits(posicaoPara, 2);  // Mostra só os dois bytes

    escreverMultiplosCoils(0, 16, 2, posicaoDe);  // Envia sempre 16 coils, começando do endereço 0
    sleep(1);  // espera 1 segundo
    escreverMultiplosCoils(0, 16, 2, posicaoPara);  // Envia sempre 16 coils, começando do endereço 0
}
