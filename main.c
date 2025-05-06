#include <stdio.h>
#include "teste.h"
#include "writecoils.h"
#include "mapa.h"
#include "falhas.h"

#define PORTA_COM "/dev/ttyUSB0"
#define BAUDRATE 9600

void mostrarHeader() {
    printf("=========================================\n");
    printf("MODBUS RTU MASTER - PEDRO HENRIQUE DE ASSUMPÇÃO\n");
    printf("=========================================\n\n");
}

void mostrarMenu() {
    printf("1. Escrever múltiplos coils (0x0F)\n");
    printf("2. Enviar comando com endereço inválido (teste de timeout)\n");
    printf("3. Enviar função inválida (teste de exceção 01)\n");
    printf("4. Enviar registrador inválido (teste de exceção 02)\n");
    printf("5. Enviar valor inválido (teste de exceção 03)\n");
    printf("6. Enviar quadro com CRC inválido (teste de erro de integridade)\n");
    printf("x. Sair\n\n");
}

int main() {
    mostrarHeader();
    
    char opcao;

    do {
        mostrarMenu();
        printf("Digite a opção desejada: ");
        scanf(" %c", &opcao);  // espaço ignora ENTER anterior

        switch (opcao) {
            case '1':
                printf("\n1. Escrever múltiplos coils (0x0F)\n");
                sendPosicao(); // writecoils.h
                break;
            case '2':
                printf("\n2. Enviar comando com endereço inválido (teste de timeout)\n");
                enviarEnderecoInvalido();
                break;
            case '3':
                printf("\n3. Enviar função inválida (teste de exceção 01)\n");
                enviarFuncaoInvalida();
                break;
            case '4':
                printf("\n4. Enviar registrador inválido (teste de exceção 02)\n");
                enviarRegistradorInvalido();
                break;
            case '5':
                printf("\n5. Enviar valor inválido (teste de exceção 03)\n");
                enviarValorInvalido();
                break;
            case '6':
                printf("\n6. Enviar quadro com CRC inválido (teste de erro de integridade)\n");
                enviarCRCInvalido();
                break;
            case 'T':
            case 't':
                printf("\nTeste Fixo\n");
                teste();
                break;
            case 'l':
            case 'L':
                printf("\nTeste Mapa\n");
                testeMapaCoils();
                break;
            case 'w':
            case 'W':
                printf("\nTeste Write Coils\n");
                testeMultiplosCoils();
                break;
            case 'x':
            case 'X':
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        printf("\n");

    } while (opcao != 'x' && opcao != 'X');

    return 0;
}

