#ifndef FALHAS_H
#define FALHAS_H

/**
 * @brief Envia resposta de exceção: Endereço de escravo inválido.
 */
void enviarEnderecoInvalido(void);

/**
 * @brief Envia resposta de exceção: Função Modbus inválida.
 */
void enviarFuncaoInvalida(void);

/**
 * @brief Envia resposta de exceção: Registrador (coils) inválido.
 */
void enviarRegistradorInvalido(void);

/**
 * @brief Envia resposta de exceção: Dado ou valor inválido.
 */
void enviarValorInvalido(void);

/**
 * @brief Envia resposta de exceção: Erro de verificação de CRC.
 */
void enviarCRCInvalido(void);

#endif // FALHAS_H
