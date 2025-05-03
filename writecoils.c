#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600
#define TIMEOUT_MS 1000


int posicaoParaIndice(char coluna, char linha) {
    coluna = toupper(coluna);
    if (coluna < 'A' || coluna > 'H' || linha < '1' || linha > '8') return -1;
    int col = coluna - 'A';
    int lin = linha - '1';
    return lin * 8 + col;
}

void escreverMultiplosCoils() {
    char comando[5];
    printf("Digite o intervalo (ex: A2H8): ");
    scanf("%4s", comando);

    if (strlen(comando) != 4) {
        printf("❌ Entrada inválida. Use 4 caracteres (ex: A2H8).\n");
        return;
    }

    int inicio = posicaoParaIndice(comando[0], comando[1]);
    int fim    = posicaoParaIndice(comando[2], comando[3]);

    if (inicio == -1 || fim == -1 || inicio > fim) {
        printf("❌ Intervalo inválido.\n");
        return;
    }

    int quantidade = fim - inicio + 1;
    int byteCount = (quantidade + 7) / 8;

    unsigned char dados[8] = {0};  // máximo 64 bits
    for (int i = 0; i < quantidade; i++) {
        int bit = i;
        dados[bit / 8] |= (1 << (bit % 8));
    }

    unsigned char req[260] = {0};
    req[0] = 0x01;                         // ID Escravo
    req[1] = 0x0F;                         // Função
    req[2] = (inicio >> 8) & 0xFF;         // Addr High
    req[3] = inicio & 0xFF;                // Addr Low
    req[4] = (quantidade >> 8) & 0xFF;     // Qtd coils High
    req[5] = quantidade & 0xFF;            // Qtd coils Low
    req[6] = byteCount;

    memcpy(&req[7], dados, byteCount);

    unsigned short crc = CRC16(req, 7 + byteCount);
    req[7 + byteCount] = crc & 0xFF;
    req[8 + byteCount] = crc >> 8;

    int total = 9 + byteCount;

    SerialPort s;
    if (!serialOpen(&s, PORTA_COM, BAUDRATE, 8, NOPARITY, TWOSTOPBITS)) {
        fprintf(stderr, "Erro ao abrir porta serial.\n");
        return;
    }

    printf("📤 Enviando: ");
    exibeDados(req, total);
    serialPutBytes(&s, req, total);

    unsigned char buffer[20] = {0};
    int recebidos = leRespostaCompleta(&s, buffer, 8, TIMEOUT_MS);

    if (recebidos == 8) {
        printf("📥 Resposta: ");
        exibeDados(buffer, recebidos);

    } else {
        printf("⚠️ Sem resposta ou resposta incompleta (%d bytes).\n", recebidos);
    }

    serialClose(&s);
}
