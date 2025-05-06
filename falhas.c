#include "serial.h"
#include "crc16.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>


/**
 * Envia uma mensagem Modbus com funcao invalida (0x01)
 */
void enviarFuncaoInvalida(void) {
    uint8_t req[4] = {0x01, 0x01};  // ID e funcao inexistente
    unsigned short crc = CRC16(req, 2);
    req[2] = crc & 0xFF;
    req[3] = crc >> 8;

    printf("🚫 Enviando funcao invalida\n");
    modbusWrite((char *)req, 4);
}

/**
 * Envia uma mensagem Modbus com endereco de registrador invalido (0x02)
 */
void enviarRegistradorInvalido(void) {
    uint8_t req[10] = {0x01, 0x0F, 0xFF, 0xF0, 0x00, 0x10, 0x02, 0x00};
    unsigned short crc = CRC16((char *)req, 8);
    req[8] = crc & 0xFF;
    req[9] = crc >> 8;

    printf("🚫 Enviando registrador invalido\n");
    modbusWrite((char *)req, 10);
}

/**
 * Envia uma mensagem Modbus com valor de dado invalido (0x03)
 */
void enviarValorInvalido(void) {
    uint8_t req[10] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x01, 0xFF};
    unsigned short crc = CRC16((char *)req, 8);
    req[8] = crc & 0xFF;
    req[9] = crc >> 8;

    printf("🚫 Enviando valor invalido\n");
    modbusWrite((char *)req, 10);
}

/**
 * Envia uma mensagem Modbus com endereco invalido (ID de escravo inexistente)
 */
void enviarEnderecoInvalido(void) {
    uint8_t req[10] = {0x09, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x02, 0xFF};
    unsigned short crc = CRC16((char *)req, 8);
    req[8] = crc & 0xFF;
    req[9] = crc >> 8;

    printf("🚫 Enviando endereco de escravo invalido\n");
    modbusWrite((char *)req, 10);
}

/**
 * Envia uma mensagem Modbus com CRC invalido
 */
void enviarCRCInvalido(void) {
    uint8_t req[10] = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x02, 0xFF, 0x00, 0x00}; // CRC propositalmente errado
    printf("🚫 Enviando mensagem com CRC invalido\n");
    modbusWrite((char *)req, 10);
}
