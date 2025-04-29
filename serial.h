// serial.h - compatível com Linux
#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar a porta serial
typedef struct {
    int fd;                 // descritor do arquivo
    struct termios tty;    // configurações da porta
} SerialPort;

// Funções
int serialOpen(SerialPort* s, const char* portname, int baudrate, int databits, int parity, int stopbits);
int serialPutBytes(SerialPort* s, const char* data, int length);
int serialClose(SerialPort* s);
void exibeDados(const char* buffer, int length);
int recebeResposta(SerialPort* s, char* buffer, int baudrate);
int leRespostaCompleta(SerialPort* s, char* buffer, int tamanhoEsperado, int timeout_ms);

// Constantes de paridade
#define NOPARITY 0
#define PARITY_EVEN 1
#define PARITY_ODD 2

// Constantes de stopbits
#define ONESTOPBIT 1
#define TWOSTOPBITS 2

#endif