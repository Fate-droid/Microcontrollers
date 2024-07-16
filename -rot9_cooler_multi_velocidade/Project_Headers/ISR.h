/*
 * ISR.h
 *
 *  Created on: Oct 18, 2022
 *      Author: ea871
 */
#include "timers.h"
#include "GPIO_latch_lcd.h"
#include "derivative.h"


#ifndef ISR_H_
#define ISR_H_

//Ting: Se fier reordenacao ... podemos economizar alguns codigos ...
//typedef enum estado_tag {
//	DESLIGADO,
//	RESET,
//	INCREMENTA,
//	DECREMENTA
//} estado_type;
typedef enum estado_tag {
	RESET,
	INCREMENTA,
	DECREMENTA,
	DESLIGADO
} estado_type;

/**
 * @brief ???
 * @return ...
 */
estado_type ISR_leEstado();

/**
 * @brief ???
 * @param[in] s ...
 */
void ISR_escreveEstado(estado_type s);

/**
 * @brief ???
 * @return ...
 */
uint8_t ISR_leDuty();

/**
 * @brief ???
 * @param[in] ...
 */
void ISR_escreveDuty(uint8_t valor);

#endif /* ISR_H_ */
