#include <stdio.h>
#include "menu.h"
#include "teste.h"
#include "writecoils.h"
#include "mapa.h"

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
                printf("[Opção 1] Escrever múltiplos coils selecionada.\n");
                sendPosicao(); // writecoils.h
                break;
            case '2':
                enviarEnderecoInvalido();
                break;
            case '3':
                enviarFuncaoInvalida();
                break;
            case '4':
                enviarRegistradorInvalido();
                break;
            case '5':
                enviarValorInvalido();
                break;
            case '6':
                enviarCRCInvalido();
                break;
            case 'T':
            case 't':
                teste();
                break;
            case 'l':
            case 'L':
                testeMapaCoils();
            case 'w':
            case 'W':
                testeMultiplosCoils();
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

