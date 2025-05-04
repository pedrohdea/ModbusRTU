#include "serial.h"
#include "crc16.h"

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>



unsigned long millis_now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL) + (tv.tv_usec / 1000UL);
}
SerialPort portaSerial; // variável global

int serialOpen(const char *portname, int baudrate, int databits, int parity, int stopbits)
{
    portaSerial.fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (portaSerial.fd < 0)
    {
        perror("Erro ao abrir porta serial");
        return 0;
    }

    memset(&portaSerial.tty, 0, sizeof(portaSerial.tty));
    if (tcgetattr(portaSerial.fd, &portaSerial.tty) != 0)
    {
        perror("Erro em tcgetattr");
        close(portaSerial.fd);
        return 0;
    }

    speed_t speed;
    switch (baudrate)
    {
    case 9600:
        speed = B9600;
        break;
    case 19200:
        speed = B19200;
        break;
    case 38400:
        speed = B38400;
        break;
    case 57600:
        speed = B57600;
        break;
    case 115200:
        speed = B115200;
        break;
    default:
        fprintf(stderr, "Baudrate inválido. Usando 9600.\n");
        speed = B9600;
    }

    cfsetospeed(&portaSerial.tty, speed);
    cfsetispeed(&portaSerial.tty, speed);

    portaSerial.tty.c_cflag &= ~CSIZE;
    portaSerial.tty.c_cflag |= (databits == 7) ? CS7 : CS8;

    switch (parity)
    {
    case PARITY_EVEN:
        portaSerial.tty.c_cflag |= PARENB;
        portaSerial.tty.c_cflag &= ~PARODD;
        break;
    case PARITY_ODD:
        portaSerial.tty.c_cflag |= PARENB;
        portaSerial.tty.c_cflag |= PARODD;
        break;
    default:
        portaSerial.tty.c_cflag &= ~PARENB;
        break;
    }

    if (stopbits == TWOSTOPBITS)
        portaSerial.tty.c_cflag |= CSTOPB;
    else
        portaSerial.tty.c_cflag &= ~CSTOPB;

    portaSerial.tty.c_cflag |= CREAD | CLOCAL;
    portaSerial.tty.c_lflag = 0;
    portaSerial.tty.c_iflag = 0;
    portaSerial.tty.c_oflag = 0;
    portaSerial.tty.c_cc[VMIN] = 0;
    portaSerial.tty.c_cc[VTIME] = 1; //VTIME = 1 → read() espera até 100 ms por um byte.

    if (tcsetattr(portaSerial.fd, TCSANOW, &portaSerial.tty) != 0)
    {
        perror("Erro em tcsetattr");
        close(portaSerial.fd);
        return 0;
    }

    return 1;
}

int serialWrite(const char *data, int length)
{
    return write(portaSerial.fd, data, length);
}

int serialClose(void)
{
    return close(portaSerial.fd) == 0;
}

void exibeDados(const char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
}


char *lerResposta(void)
{
    static char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    int bytesRecebidos = 0;
    unsigned long inicio = millis_now();
    unsigned long ultimoByte = inicio;

    printf("🔍 Aguardando frame até %lu ms...\n", inicio + TIMEOUT_TOTAL_MS);

    while (1)
    {
        int n = read(portaSerial.fd, buffer + bytesRecebidos, 1);
        unsigned long agora = millis_now();

        if (n > 0)
        {
            ultimoByte = agora;
            bytesRecebidos += n;
        }
        else
        {
            if ((agora - ultimoByte) >= TIMEOUT_INTERBYTE_MS && bytesRecebidos > 0)
                break;

            if ((agora - inicio) >= TIMEOUT_TOTAL_MS)
            {
                printf("⏱️ Timeout global de %lu ms atingido.\n", agora - inicio);
                return NULL;
            }

            usleep(100); // evita CPU 100%
        }
    }

    if (bytesRecebidos < 5)
    {
        printf("⚠️ Resposta muito curta (%d bytes).\n", bytesRecebidos);
        return NULL;
    }

    unsigned short crc_resp = (unsigned char)buffer[bytesRecebidos - 2] |
                              ((unsigned char)buffer[bytesRecebidos - 1] << 8);
    unsigned short crc_calc = CRC16(buffer, bytesRecebidos - 2);

    if (crc_resp != crc_calc)
    {
        printf("❌ CRC inválido");
        return NULL;
    }

    if (buffer[1] & 0x80)
    {
        printf("❗ Exceção Modbus recebida: Código 0x%02X\n", buffer[2]);
        return NULL;
    }

    printf("📥 Resposta recebida (%d bytes): ", bytesRecebidos);
    exibeDados(buffer, bytesRecebidos);
    return buffer;
}
