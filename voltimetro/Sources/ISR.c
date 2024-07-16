/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Fabio e Felipe
 * @date 31/10/2022
 */

#include "derivative.h"
#include "ISR.h"
#include "ADC.h"
#include "util.h"

static uint8_t valor[2];		///< valor convertido do sensor LM61
static estado_type estado = ESPERA;

void PIT_IRQHandler(void) {	
//	GPIOE_PSOR = GPIO_PIN(21);			//!< \li \l trigger hardware enviado para ADC0
//	GPIOE_PTOR = GPIO_PIN(22);			//!< \li \l alternar o canal 2 do analisador logico
	
	estado = AMOSTRA_VOLT;					//!< \li \l estado; amostrar LM61

	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;     //!< \li \l w1c (limpa pendencias)
}


void ADC0_IRQHandler(void) {
	if( ADC0_SC1A & ADC_SC1_COCO_MASK )     // avalia se a conversão já foi completada (True se for)
	{
		if (estado == AMOSTRA_VOLT) {
//			GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por hardware
			valor[0] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;	//!< \li \l chavear para trigger por software
			ADC0_selChannel (0b11010); 			//!< \li \l selecionar o canal do sensor AN3031
//			GPIOE_PSOR = GPIO_PIN(21);			//!< trigger software enviado para ADC0
			estado = AMOSTRA_TEMP;
			
		} else if (estado == AMOSTRA_TEMP) {
//			GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por software
			valor[1] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 |= ADC_SC2_ADTRG_MASK;		//!< \li \l chavear para trigger por hardware
			ADC0_selChannel (0b01001); 			//!< \li \l selecionar o canal do header H7
			estado = ATUALIZACAO;
		}
	}
}

estado_type ISR_leEstado(){
	return estado;
}

void ISR_escreveEstado(estado_type novo){
	estado = novo;
}

void ISR_LeValoresAmostrados (uint16_t *v) {
	v[0] = valor[0];
	v[1] = valor[1];
	
	return;
}

