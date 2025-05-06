#ifndef MAPA_H
#define MAPA_H

#include <stdint.h>

/**
 * @brief Gera dois vetores de 16 bytes representando a ativação de colunas e linhas.
 *
 * A jogada (ex: "A2H8") é convertida em dois vetores:
 * - resultadoColunas[16]: bits ativos das colunas (A-H) dos dois pontos;
 * - resultadoLinhas[16]: bits ativos das linhas (1-8) dos dois pontos.
 *
 * @param comando String com 4 caracteres representando uma jogada (ex: "A2H8").
 * @param resultadoColunas Vetor de 16 bytes para ativação das colunas.
 * @param resultadoLinhas Vetor de 16 bytes para ativação das linhas.
 */
void gerarMapaCoils(const char *comando, uint8_t resultadoColunas[16], uint8_t resultadoLinhas[16]);

/**
 * @brief Exibe os bits de um vetor de bytes no console.
 *
 * @param dados Vetor de dados a ser exibido.
 * @param tamanho Número de bytes no vetor.
 */
void exibeBits(uint8_t *dados, int tamanho);

/**
 * @brief Função de teste interativa para o mapa de coils.
 * Solicita uma jogada ao usuário e imprime o mapa de colunas e linhas.
 */
void testeMapaCoils(void);

#endif
