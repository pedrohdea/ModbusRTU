#ifndef MAPA_H
#define MAPA_H

#include <stdint.h>

/**
 * @brief Converte uma jogada tipo "A2H8" em um vetor de 16 bytes
 *        representando bits de colunas e linhas ativadas.
 *
 * @param comando Ponteiro para uma string de 4 caracteres (ex: "A2H8").
 * @param resultado Vetor de 16 posições a ser preenchido.
 */
void gerarMapaCoils(const char *comando, uint8_t resultado[16]);

/**
 * @brief Imprime os bits de um vetor de bytes, útil para depuração.
 *
 * @param dados Vetor de bytes.
 * @param tamanho Quantidade de bytes a exibir.
 */
void exibeBits(uint8_t *dados, int tamanho);

/**
 * @brief Função de teste para gerar e imprimir o mapa de coils a partir de uma jogada.
 */
void testeMapaCoils(void);

#endif // MAPA_H
