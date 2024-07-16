/*
 * ISR.c
 *
 *  Created on: Mar 10, 2023
 *      Author: Fabio
 */

#include "ISR.h"
#include "ADC.h"
static tipo_estado estado;
static uint16_t valor[2];		///< valor convertido do sensor LM61

void ADC0_IRQHandler(void) {
	if( ADC0_SC1A & ADC_SC1_COCO_MASK )
	{
		if (estado == AMOSTRA_VOLT) {
		//	GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por hardware
			valor[0] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;	//!< \li \l chavear para trigger por software
			ADC_selecionaCanal (0b11010); 			//!< \li \l selecionar o canal do sensor AN3031
			//GPIOE_PSOR = GPIO_PIN(21);			//!< trigger software enviado para ADC0
			estado = AMOSTRA_TEMP;
		} else if (estado == AMOSTRA_TEMP) {
		//	GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por software
			valor[1] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 |= ADC_SC2_ADTRG_MASK;		//!< \li \l chavear para trigger por hardware
			ADC_selecionaCanal (0b01001); 			//!< \li \l selecionar o canal do header H7
			estado = ATUALIZACAO;
		}
	}
}

tipo_estado ISR_LeEstado(){
	return estado;
	
}

void ISR_EscreveEstado(tipo_estado flag){
	estado = flag;
	
}

uint16_t *ISR_leValoresAmostrados(){
	return &valor; 
}
