#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600
#define TIMEOUT_INTERBYTE_MS 4  // 3.5 bytes @ 9600bps ≈ 4ms

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

    if (quantidade > 64) {
        printf("❌ Máximo de 64 coils por requisição.\n");
        return;
    }

    unsigned char dados[8] = {0};
    for (int i = 0; i < quantidade; i++) {
        dados[i / 8] |= (1 << (i % 8));  // LSB-first (padrão Modbus)
    }

    unsigned char req[260] = {0};
    req[0] = 0x01;                         // ID Escravo
    req[1] = 0x0F;                         // Função
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

    if (!serialOpen(PORTA_COM, BAUDRATE, 8, NOPARITY, TWOSTOPBITS)) {
        fprintf(stderr, "❌ Erro ao abrir porta serial.\n");
        return;
    }


    printf("📤 Enviando: ");
    exibeDados((char *)req, total);
    serialWrite((char *)req, total);

    char *resposta = lerResposta();
    if (resposta) {
        printf("✅ Resposta válida recebida.\n");
    } else {
        printf("❌ Nenhuma resposta válida.\n");
    }

    serialClose();
}
