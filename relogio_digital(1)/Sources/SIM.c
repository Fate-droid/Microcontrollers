/*
 * @name SIM.c
 * @brief Definicao de funcoes de derivacao de sinais de relogio
 * @date Jan 16, 2023
 * @author Wu Shin-Ting
 */
#include "derivative.h"

void SIM_setaOUTDIV4 (uint8_t OUTDIV4) {
	/*
	 * Configurar a frequencia do clock de barramento atraves do divisor da frequencia 
	 * do sinal MCGOUTCLK  (clock de barramento = MCGOUTCLK/(OUTDIV1*OUTDIV4))
	 */
	SIM_CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4(0b111); //!< \li \l setar OUTDIV4
	SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(OUTDIV4); 	
}


