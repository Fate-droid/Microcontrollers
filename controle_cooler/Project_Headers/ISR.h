/*
 * ISR.h
 *
 *  Created on: Mar 10, 2023
 *      Author: Fabio
 */

#ifndef ISR_H_
#define ISR_H_
#include "stdint.h"
typedef enum estado_tag{
	AMOSTRA_VOLT,
	AMOSTRA_TEMP,
	ATUALIZACAO
} tipo_estado;

tipo_estado ISR_LeEstado();
uint16_t *ISR_leValoresAmostrados();

void ISR_EscreveEstado(tipo_estado flag);
#endif /* ISR_H_ */
