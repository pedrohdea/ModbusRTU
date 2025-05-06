#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>

// Paridades
#define NOPARITY    0
#define PARITY_EVEN 1
#define PARITY_ODD  2

// Stop Bits
#define ONESTOPBIT   1
#define TWOSTOPBITS  2

// Timeout padrão (ms)
#define TIMEOUT_INTERBYTE_MS 4
#define TIMEOUT_TOTAL_MS     10000 //10seg

// Estrutura da porta
typedef struct {
    int fd;
    struct termios tty;
} SerialPort;

// Variável global
extern SerialPort portaSerial;

// Assinaturas das funções
int serialOpen(const char *portname, int baudrate, int databits, int parity, int stopbits);
int serialWrite(const char *data, int length);
int serialClose(void);

void exibeDados(const char *buffer, int length);
char *lerResposta(void);
int modbusWrite(const char *req, int total);

#endif
