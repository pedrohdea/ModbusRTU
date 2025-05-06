#include "mapa.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include <stdint.h>
#include <ctype.h>

/**
 * @brief Converte uma jogada tipo "A2H8" em dois vetores de 16 bytes
 *        representando a ativação de colunas e linhas para duas casas.
 *
 * @param comando Ponteiro para string com exatamente 4 caracteres (ex: "A2H8").
 * @param resultadoA Vetor de 16 posições representando a primeira casa (ex: A2).
 * @param resultadoB Vetor de 16 posições representando a segunda casa (ex: H8).
 */
void gerarMapaUnitario(const char *comando, uint8_t resultado[2]) {
    resultado[0] = 0;
    resultado[1] = 0;

    if (!comando || strlen(comando) != 2)
        return;

    char col = toupper(comando[0]);
    char lin = comando[1];

    if (col >= 'A' && col <= 'H')
        resultado[0] |= (1 << (col - 'A'));

    if (lin >= '1' && lin <= '8')
        resultado[1] |= (1 << (lin - '1'));
}

void gerarMapaCoils(const char *comando, uint8_t resultadoA[16], uint8_t resultadoB[16]) {
    memset(resultadoA, 0, 16);
    memset(resultadoB, 0, 16);

    if (!comando || strlen(comando) != 4)
        return;

    char pos1[3] = {0};  // ex: "B2"
    char pos2[3] = {0};  // ex: "H8"

    strncpy(pos1, comando, 2);
    strncpy(pos2, comando + 2, 2);

    uint8_t tempA[2] = {0}, tempB[2] = {0};
    gerarMapaUnitario(pos1, tempA);  // posição A2
    gerarMapaUnitario(pos2, tempB);  // posição H8

    resultadoA[0] = tempA[0];
    resultadoA[1] = tempA[1];

    resultadoB[0] = tempB[0];
    resultadoB[1] = tempB[1];
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
    printf("Jogada: B2H8\n");
    char comando[5] = "B2H8";
    uint8_t resultadoA[16] = {0};
    uint8_t resultadoB[16] = {0};

    gerarMapaCoils(comando, resultadoA, resultadoB);

    printf("🔎 Colunas ativadas por posição 1:");
    exibeBits(resultadoA, 2);
    printf("🔎 Colunas ativadas por posição 2:");
    exibeBits(resultadoB, 2);
}
