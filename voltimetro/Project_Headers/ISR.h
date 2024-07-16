/*!
 * @file ISR.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo ISR.c 
 * @author Wu Shin Ting
 * @date 06/03/2022
 */

#ifndef ISR_H_
#define ISR_H_

#include "stdint.h"
#include "util.h"

/**
 * @brief enumeração de tipo de dados estado_type; nomes indicam o estado do programa (
 * ver fluxo de estados no roteiro11)
 */
typedef enum state_tag {
	ESPERA,
	AMOSTRA_TEMP,
	AMOSTRA_VOLT,
	ATUALIZACAO
} estado_type;

/**
 * @brief Le estado do sistema
 * @return estado do sistema
 */
estado_type ISR_leEstado(); 


/**
 * @brief Escreve estado do sistema
 * @return novo estado do sistema
 */
void ISR_escreveEstado(estado_type novo);

/**
 * @brief Le 2 valores amostrados
 * @param[out] amostras 2 amostras
 */
void ISR_LeValoresAmostrados (uint16_t *v);

#endif /* ISR_H_ */
