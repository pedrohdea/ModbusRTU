#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600
#define PARIDADE NOPARITY
#define STOPBITS TWOSTOPBITS
#define TAMANHO_FRAME 8

int teste() {
    if (!serialOpen(PORTA_COM, BAUDRATE, 8, NOPARITY, TWOSTOPBITS)) {
        fprintf(stderr, "❌ Erro ao abrir porta serial.\n");
        return EXIT_FAILURE;
    }

    // Monta requisição Modbus: 01 0E 00 02 FF 00 + CRC
    unsigned char req[TAMANHO_FRAME] = {0x01, 0x0E, 0x00, 0x02, 0xFF, 0x00};
    unsigned short crc = CRC16((char *)req, 6);
    req[6] = crc & 0xFF;
    req[7] = crc >> 8;

    printf("📤 Dados enviados: ");
    exibeDados((char *)req, TAMANHO_FRAME);

    clock_t t1 = clock();
    serialWrite((char *)req, TAMANHO_FRAME);
    clock_t t2 = clock();
    long envio_ms = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    printf("⏱️  Tempo de envio: %ld ms\n", envio_ms);

    char *resposta = lerResposta();
    if (resposta) {
        printf("✅ Resposta válida recebida.\n");
    } else {
        printf("❌ Nenhuma resposta válida.\n");
    }

    serialClose();
    getchar();
    return 0;
}
