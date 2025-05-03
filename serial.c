// serial.c - implementação Linux
#include "serial.h"
#include <errno.h>
#include <time.h>
#include <ctype.h>

int serialOpen(SerialPort *s, const char *portname, int baudrate, int databits, int parity, int stopbits)
{
    s->fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (s->fd < 0)
        return 0;

    memset(&s->tty, 0, sizeof(s->tty));
    if (tcgetattr(s->fd, &s->tty) != 0)
        return 0;

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
        speed = B9600;
    }

    cfsetospeed(&s->tty, speed);
    cfsetispeed(&s->tty, speed);

    s->tty.c_cflag = (s->tty.c_cflag & ~CSIZE);
    switch (databits)
    {
    case 7:
        s->tty.c_cflag |= CS7;
        break;
    case 8:
    default:
        s->tty.c_cflag |= CS8;
        break;
    }

    switch (parity)
    {
    case PARITY_EVEN:
        s->tty.c_cflag |= PARENB;
        s->tty.c_cflag &= ~PARODD;
        break;
    case PARITY_ODD:
        s->tty.c_cflag |= PARENB;
        s->tty.c_cflag |= PARODD;
        break;
    case NOPARITY:
    default:
        s->tty.c_cflag &= ~PARENB;
        break;
    }

    switch (stopbits)
    {
    case TWOSTOPBITS:
        s->tty.c_cflag |= CSTOPB;
        break;
    case ONESTOPBIT:
    default:
        s->tty.c_cflag &= ~CSTOPB;
        break;
    }

    s->tty.c_cflag |= CREAD | CLOCAL;
    s->tty.c_lflag = 0;
    s->tty.c_iflag = 0;
    s->tty.c_oflag = 0;
    s->tty.c_cc[VMIN] = 0;
    s->tty.c_cc[VTIME] = 10;

    if (tcsetattr(s->fd, TCSANOW, &s->tty) != 0)
        return 0;

    return 1;
}

int serialPutBytes(SerialPort *s, const char *data, int length)
{
    return write(s->fd, data, length);
}

int serialClose(SerialPort *s)
{
    return close(s->fd) == 0;
}

void exibeDados(const char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02X ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

int recebeResposta(SerialPort *s, char *buffer, int baudrate)
{
    int total = 0;
    int timeout_ms = 2000; // 2 segundos
    clock_t start = clock();

    while (((clock() - start) * 1000 / CLOCKS_PER_SEC) < timeout_ms)
    {
        int n = read(s->fd, buffer + total, 1);
        if (n > 0)
        {
            total += n;
            if (total >= 5 && buffer[1] != 0x05)
                break; // suposição simples
        }
    }

    return total;
}

int leRespostaCompleta(SerialPort *s, char *buffer, int tamanhoEsperado, int timeout_ms)
{
    int total = 0;
    clock_t start = clock();
    long timeout_ticks = (timeout_ms * CLOCKS_PER_SEC) / 1000;

    printf("\n🔍 Aguardando resposta (%d bytes)...\n", tamanhoEsperado);

    while (total < tamanhoEsperado)
    {
        clock_t now = clock();

        int n = read(s->fd, buffer + total, tamanhoEsperado - total);
        if (n > 0)
        {
            for (int i = 0; i < n; i++)
            {
                printf("[+%ld ticks] Byte %d: 0x%02X (%c)\n",
                       (long)(now - start),
                       total + i,
                       (unsigned char)buffer[total + i],
                       isprint((unsigned char)buffer[total + i]) ? buffer[total + i] : '.');
            }
            total += n;
            start = clock(); // reinicia timeout a cada byte recebido
        }
        else
        {
            if ((now - start) > timeout_ticks)
            {
                printf("⏱️ Timeout de %dms atingido.\n", timeout_ms);
                break;
            }
            printf("[nada]\n");
            usleep(100); // aguarda para não travar CPU
        }
    }

    printf("📥 Total de bytes recebidos: %d\n", total);
    unsigned short crc_resp = buffer[total - 2] | (buffer[total - 1] << 8);
    unsigned short crc_calc = CRC16(buffer, total - 2);

    if (crc_resp == crc_calc)
        printf("✅ CRC OK.\n");
    else
        printf("❌ CRC inválido. Calc: 0x%04X, Resp: 0x%04X\n", crc_calc, crc_resp);
    return total;
}
