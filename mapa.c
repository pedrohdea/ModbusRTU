#include "mapa.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void gerarMapaCoils(const char *comando, uint8_t resultado[16]) {
    memset(resultado, 0, 16);  // Zera o vetor

    if (strlen(comando) != 4) return;

    char col1 = toupper(comando[0]);
    char lin1 = comando[1];
    char col2 = toupper(comando[2]);
    char lin2 = comando[3];

    // Colunas A-H mapeadas nos bits de resultado[0]
    if (col1 >= 'A' && col1 <= 'H')
        resultado[0] |= (1 << (col1 - 'A'));
    if (col2 >= 'A' && col2 <= 'H')
        resultado[0] |= (1 << (col2 - 'A'));

    // Linhas 1-8 mapeadas nos bits de resultado[1]
    if (lin1 >= '1' && lin1 <= '8')
        resultado[1] |= (1 << (lin1 - '1'));
    if (lin2 >= '1' && lin2 <= '8')
        resultado[1] |= (1 << (lin2 - '1'));
}

void exibeBits(uint8_t *dados, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        for (int bit = 0; bit < 8; bit++) {
            printf("%d", (dados[i] >> bit) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void testeMapaCoils(void) {
    char comando[5];
    printf("Digite a jogada (ex: A2H8): ");
    scanf("%4s", comando);
    uint8_t resultado[16] = {0};

    gerarMapaCoils(comando, resultado);

    printf("🔎 Mapa de coils gerado para o comando \"%s\":\n", comando);
    exibeBits(resultado, 2);
}
