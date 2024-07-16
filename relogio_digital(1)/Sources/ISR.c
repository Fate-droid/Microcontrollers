/*
 * @file ISR.c
 * @brief Definicao de prototipos relativos a rotinas de servico do projeto relogio_digital
 * @date Jan 16, 2023
 * @author Wu Shin-Ting
 */
#include "derivative.h"
#include "ISR.h"
#include "RTC.h"
#include "PIT.h"
#include "util.h"

static tipo_estado estado;
static uint32_t hor[4];

void ISR_carregaHorario() {
	uint32_t segundos;

	RTClpo_getTime (&segundos);
	ConvertSectoDay(segundos, &hor[3], &hor[0], &hor[1], &hor[2]);
}

void ISR_leHorario (uint32_t *dias, uint32_t *horas, uint32_t *minutos, uint32_t *segundos)
{
		*dias = hor[3];
		*horas = hor[0];
		*minutos = hor[1];
		*segundos = hor[2];

		return;
}

tipo_estado ISR_LeEstado ()
{	
	return estado;
}

void ISR_escreveEstado (tipo_estado s)
{	
	estado = s;
}

//void PIT_IRQHandler () {
//	uint32_t segundos;
//	
//	counter++;
//	
//	if (counter >= POSTSCALER) {
//		ConvertDaytoSec (hor[3], hor[0], hor[1], hor[2], &segundos);
//		RTClpo_setTime (segundos);
//		
//		//w1c
//		PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
//		
//		PIT_desativaTimer0 ();
//		estado = NORMAL;	
//		
//		return;
//	}
//	
//	//w1c
//	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
//}

void PIT_IRQHandler () {
	uint32_t segundos;
	
	ConvertDaytoSec (hor[3], hor[0], hor[1], hor[2], &segundos);
	RTClpo_setTime (segundos);
	
	//w1c
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
	
	PIT_desativaTimer0 ();
	estado = NORMAL;	
		
	
}

void PORTA_IRQHandler () {
	//Reativa PIT 
	PIT_desativaTimer0();
	PIT_ativaTimer0();

	switch (estado) {
	case NORMAL:
		ISR_carregaHorario();

		if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(4))) {
			estado = INCREMENTE_HORA;
			
			//Limpar o estado de solicitacao
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(4)); 
		} 
		if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(5))) {
			estado = INCREMENTE_MINUTO;
			
			//Limpar o estado de solicitacao
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(5)); 
		} 
		if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(12))) {
			estado = INCREMENTE_SEGUNDO;
			
			//Limpar o estado de solicitacao
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(12)); 
		} 
		
		break;
	case INCREMENTE_HORA:
	case INCREMENTE_HORA_ESPERA:
		if (PORTA_ISFR / PORT_ISFR_ISF(GPIO_PIN(4))){
			hor[0]++;
			if (hor[0]>=24) hor[0] = 0;
			estado = INCREMENTE_HORA;
			
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(4));
		}
		
		break;
	case INCREMENTE_MINUTO:
	case INCREMENTE_MINUTO_ESPERA:
		if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(5))) {
			hor[1]++;
			if (hor[1] >= 60) hor[1] = 0;
			estado = INCREMENTE_MINUTO;
			
			//Limpar o estado de solicitacao
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(5)); 
		} 
		break;
	case INCREMENTE_SEGUNDO:
	case INCREMENTE_SEGUNDO_ESPERA:
		if (PORTA_ISFR & PORT_ISFR_ISF(GPIO_PIN(12))){
			hor[2]++;
			if (hor[2] >=60) hor[2]=0;
			estado = INCREMENTE_SEGUNDO;
			
			PORTA_ISFR |= PORT_ISFR_ISF(GPIO_PIN(12));
		}
		break;
	}
	
}

