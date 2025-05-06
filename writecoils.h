#ifndef WRITECOILS_H
#define WRITECOILS_H

#include <stdint.h>

/**
 * @brief Envia uma requisição Modbus RTU para escrever múltiplos coils (função 0x0F).
 *
 * @param inicio Índice inicial do primeiro coil (0 a 65535).
 * @param quantidade Número total de coils a serem escritos (1 a 64).
 * @param byteCount Número de bytes necessários para representar os coils.
 * @param dados Vetor de bytes com os valores dos coils (formato LSB-first).
 * @return int Retorna 1 em caso de sucesso, 0 em caso de erro.
 */
int escreverMultiplosCoils(int inicio, int quantidade, int byteCount, const unsigned char *dados);

/**
 * @brief Lê uma jogada do usuário e aciona os coils correspondentes no tabuleiro.
 *
 * A jogada deve estar no formato "A2H8", e os bits ativados serão enviados via Modbus.
 */
void sendPosicao(void);

/**
 * @brief Converte uma coordenada (coluna e linha) para um índice único de 0 a 63.
 *
 * @param coluna Letra da coluna (A-H).
 * @param linha Dígito da linha (1-8).
 * @return int Índice calculado ou -1 se inválido.
 */
int posicaoParaIndice(char coluna, char linha);

#endif // WRITECOILS_H
