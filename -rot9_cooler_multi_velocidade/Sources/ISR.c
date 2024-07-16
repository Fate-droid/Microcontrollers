/*
 * ISR.c
 *
 *  Created on: Oct 18, 2022
 *      Author: ea871
 */

#include "ISR.h"
#include "timers.h"
#include "GPIO_latch_lcd.h"
#include "util.h"
#include "derivative.h"
#include "GPIO_Switches.h"

static estado_type estado = DESLIGADO;
static uint8_t duty = 0;

//Ting: Precisa ser visivel por todas as funcoes???
//Nao poderia ser uma variavel local?
//static uint32_t valor;

estado_type ISR_leEstado(){
	return estado;
}

void ISR_escreveEstado(estado_type s){
	estado = s;
}

uint8_t ISR_leDuty(){
	return duty;
}

void ISR_escreveDuty(uint8_t valor){
	duty = valor;
}

void PORTA_IRQHandler(){
	uint32_t valor;

	delay_2us(100000);
	GPIO_leSwitchesISF (&valor);
	
	switch (estado){
		case DESLIGADO:
			if (valor == 0x0010) {
				duty=25; estado = RESET;
			}	
			break;
		case RESET:
			if (valor == 0x0010) {
				duty=0; estado = DESLIGADO;
			}
			else if (valor == 0x0020) { 
				duty -= 4; estado = DECREMENTA;
				//Ting: o que acontece quando fica "negativo" (resposta do item 1(d)!)
				if (duty > 100) duty = 0;
			}
			else if (valor == 0x1000) { 
				duty += 4; estado = INCREMENTA;
				//Ting: o que acontece quando fica maior que 100%?
				if (duty > 100) duty = 100;
			}		
			break;
		case INCREMENTA:
			if (valor == 0x0010) {
				duty=0; estado = DESLIGADO;
			}
			else if (valor == 0x0020) {
				duty -= 4; estado = DECREMENTA;
				//Ting: Facam devidos ajustes ...
				if (duty > 100) duty = 0;
			}
			else if (valor == 0x1000) {
				//Ting: Faltou este bloco!!!
				duty +=4; estado = INCREMENTA;
				if (duty > 100) duty = 100;
			}
			else if (valor == 0x1020) {
				duty = 25; estado = RESET;
			}	
			break;
		case DECREMENTA:
			if (valor == 0x0010) {
				duty = 0; estado = DESLIGADO;
			}
			else if (valor == 0x0020) { 
				duty -= 4; estado = DECREMENTA;
				if (duty > 100) duty = 0;
			}
			else if (valor == 0x1000) {
				duty +=4; estado = INCREMENTA;
				//Ting: Facam devidos ajustes ...
				if (duty > 100) duty = 100;
			}
			else if (valor == 0x1020) {
				duty = 25; estado = RESET;
			}
			break;
	}
	
	//Limpa as flags de interrupção
	PORTA_ISFR |= (0x1030);
}

	
	

