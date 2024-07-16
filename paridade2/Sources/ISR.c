/*
 * ISR.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Fabio
 */

#include "buffer_circular.h"
#include "ISR.h"
#include "derivative.h"

#define TAMMAX 80 //80 bytes alocados para cada buffer
static BufferCirc_type bufferE,bufferS;
static tipo_estado estado;

void ISR_inicializaBC(){
	BC_init(&bufferE, TAMMAX);
	BC_init(&bufferS, TAMMAX);
}

void ISR_EnviaString (char *string) {
	uint8_t i;
	
	while (BC_push( &bufferS, string[0])==-1);	///espera ate haver espaco no buffer c
	UART0_C2 |= UART0_C2_TIE_MASK;
	
	//while (BC_push( &buffer2, string[0])==-1);
	//UART2_C2 |= UART_C2_TIE_MASK;
	i=1;
	while (string[i] != '\0') {
		while (BC_push( &bufferS, string[i])==-1);
		i++;
	}
}

void ISR_extraiString (char *string) {
	//Entrada de uma nova string
	uint8_t i=0;
	BC_pop (&bufferE, &string[i]);
	while (string[i] != '\0') {
		BC_pop (&bufferE, &string[++i]);				
	}
}


void UART0_IRQHandler()
{
	char item;
	if (UART0_S1 & UART0_S1_RDRF_MASK) {	//habilitado para leitura
		item= UART0_D;
		if (estado != EXPRESSAO) return;	//se nao se esta no estado adequado nao processa o dado
		
		UART0_D = item;
		if (item == '\r') {
			BC_push (&bufferE, '\0');
			while (!(UART0_S1 & UART_S1_TDRE_MASK)); 		//Enquanto houver transmissao espera
			UART0_D = '\n';
			ISR_escreveEstado(TOKENS);
			
		} else {
			BC_push (&bufferE, item);
			}
		} 
	else if (UART0_S1 & UART0_S1_TDRE_MASK) {
		//Interrupcao solicitada pelo canal Tx
		if (BC_isEmpty(&bufferS)){
			UART0_C2 &= ~UART0_C2_TIE_MASK;
			if ((estado == RESULTADO)){
				estado = MENSAGEM;
			}
			else if((estado ==ERRO)){
				estado = MENSAGEM;
			}
		}
		else {
			BC_pop (&bufferS, &item);
			UART0_D = item;
		}
	}
}
tipo_estado ISR_leEstado(){
	return estado;
}
void ISR_escreveEstado(uint8_t novo){
	estado = novo;
}
