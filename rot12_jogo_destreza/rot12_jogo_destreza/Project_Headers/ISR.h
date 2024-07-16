/*!
 * @file ISR.h
 * @brief Este modulo contem os programas de tratamento de instrução usados; como também as funções de leitura e escrita de estados do jogo, do arquivo ISR.c.
 * @author Fabio e Felipe
 * @date 16/11/2022
 */

#ifndef ISR_H_
#define ISR_H_

#include "stdint.h"

/**
 * @brief criação do tipo de estado estado_type que enumera os possíveis estados do jogo
 */
typedef enum state_tag {
	INICIAL,
	RANDOM,
	LEDSWAIT,
	LEDSGO,
	CONTAGEM,
	CONTAGEMWAIT,
	RESULTADO
} estado_type;

/*!
 * @brief : reseta a variavel timeout em 0
 */
void ISR_ResetTimeout ();
/**
 * @brief Rotina de servico do TPM modulo 1.
 */
void FTM1_IRQHandler ();
/**
 * @brief Rotina de servico do TPM modulo 2.
 */
void FTM2_IRQHandler ();
/**
 * @brief Rotina de servico do TPM modulo 0.
 */
void FTM0_IRQHandler ();

/*!
 * @brief : atribui um valor para variavel estado
 * @param[in]: valor do estado desejado
 */
void ISR_EscreveEstado(estado_type valor);
/*!
 * @brief : retorna o valor da variavel estado
 */
estado_type ISR_LeEstado ();

/*!
 * @brief : atribui um valor para a variavel multiplos
 * @param[in] valor
 */
void ISR_EscreveMultiplos(uint8_t valor);

/*!
 * @brief : função void que devolve parametros da medição
 * @param[out]: valor da botoeira que foi pressionada primeiro.
 * @param[out]: valor de contagem quando foi pressionada a botoeira NMI.
 * @param[out]: valor da contagem quando foi pressionada a botoeira IRQA5.
 * @param[out]: número de ciclos do contador entre os dois apertos entre as duas botoeiras.
 * 
 */
void ISR_LeContagem (uint8_t *botoeira_arg, uint16_t *primeiro, uint16_t *segundo, uint16_t *ciclos);

#endif /* ISR_H_ */
