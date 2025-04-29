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
#define TIMEOUT_MS 1000

int teste() {
    SerialPort s;

    if (!serialOpen(&s, PORTA_COM, BAUDRATE, 8, PARIDADE, STOPBITS)) {
        fprintf(stderr, "Erro ao abrir porta serial.\n");
        return EXIT_FAILURE;
    }

    // Monta requisição Modbus: 01 05 00 02 FF 00 + CRC
    char req[TAMANHO_FRAME] = {0x01, 0x05, 0x00, 0x02, 0xFF, 0x00};
    unsigned short crc = CRC16(req, 6);
    req[6] = crc & 0xFF;
    req[7] = crc >> 8;

    printf("📤 Dados enviados: ");
    exibeDados(req, TAMANHO_FRAME);

    clock_t t1 = clock();
    serialPutBytes(&s, req, TAMANHO_FRAME);
    clock_t t2 = clock();
    long envio_ms = (t2 - t1) * 1000 / CLOCKS_PER_SEC;
    printf("⏱️  Tempo de envio: %ld ms\n", envio_ms);

    // Aguarda resposta
    char resp[20] = {0};
    int recebidos = leRespostaCompleta(&s, resp, TAMANHO_FRAME, TIMEOUT_MS);

    if (recebidos == TAMANHO_FRAME) {
        printf("📥 Dados recebidos: ");
        exibeDados(resp, recebidos);
        crc = CRC16(resp, recebidos);
        printf(crc == 0 ? "✅ CRC-16 OK.\n" : "❌ CRC-16 Incorreto.\n");
    } else {
        printf("⚠️  Frame incompleto (%d bytes recebidos)\n", recebidos);
    }

    serialClose(&s);
    getchar();
    return 0;
}