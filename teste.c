#include "serial.h"
#include "crc16.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "mapa.h"
#include "writecoils.h"
#include <string.h>

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600
#define PARIDADE NOPARITY
#define STOPBITS TWOSTOPBITS
#define TAMANHO_FRAME 8

// int teste_leds(){
//     // comando de A2H8;
//     escreverMultiplosCoils();
// }

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

void testeMultiplosCoils(void) {
    char bits[17];  // 16 bits + terminador nulo
    printf("🧪 Digite os 16 bits (ex: 1000000101000001): ");
    scanf("%16s", bits);

    if (strlen(bits) != 16) {
        printf("❌ Você deve digitar exatamente 16 bits (0 ou 1).\n");
        return;
    }

    uint8_t dados[2] = {0};

    // Converte bits string em LSB-first para Modbus
    for (int i = 0; i < 16; i++) {
        if (bits[i] == '1') {
            dados[i / 8] |= (1 << (i % 8));
        }
    }

    printf("📍 Endereço inicial: 0\n");
    printf("📏 Quantidade de coils: 16\n");
    printf("📦 ByteCount: 2\n");
    printf("📤 Dados para envio: ");
    exibeBits(dados, 2);

    escreverMultiplosCoils(0, 16, 2, dados);
}
