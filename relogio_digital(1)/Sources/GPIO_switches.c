/*
 * @file GPIO_switches.c
 * @brief Definicao das funcoes declaradas em GPIO_switches.h
 * @date Jan 13, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "util.h"

void GPIO_initSwitches (uint8_t IRQC, uint8_t prioridade) {
	// Habilita o clock do modulo PORTA
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;				// para botoeiras   					// para leds

	// Funcao GPIO
	PORTA_PCR4 &= ~PORT_PCR_MUX(0x7);			// limpar os bits (0x40049010)
	PORTA_PCR4 |= PORT_PCR_MUX(0x1);			// antes de setar 0b001
	PORTA_PCR5 &= ~PORT_PCR_MUX(0x7);
	PORTA_PCR5 |= PORT_PCR_MUX(0x1);
	PORTA_PCR12 &= ~PORT_PCR_MUX(0x7);
	PORTA_PCR12 |= PORT_PCR_MUX(0x1);

	// Sentido do sinal: entrada 
	GPIOA_PDDR &= ~(GPIO_PDDR_PDD(GPIO_PIN(4)));    
	GPIOA_PDDR &= ~(GPIO_PDDR_PDD(GPIO_PIN(5))); 
	GPIOA_PDDR &= ~(GPIO_PDDR_PDD(GPIO_PIN(12))); 

	// Configura modo de interrupcao
	PORTA_PCR4 |= PORT_PCR_ISF_MASK |
			PORT_PCR_IRQC(IRQC );           	// modo de interrupcao
	PORTA_PCR5 |= PORT_PCR_ISF_MASK |
				PORT_PCR_IRQC(IRQC );           	// modo de interrupcao
	PORTA_PCR12 |= PORT_PCR_ISF_MASK |
				PORT_PCR_IRQC(IRQC );           	// modo de interrupcao


	/**
	 * Configura o modulo NVIC: habilita IRQ 30 e limpa pendencias IRQ 30
	 */
	NVIC_ISER = NVIC_ISER_SETENA(GPIO_PIN(30));            		// Habilita interrupcao PORTA
	NVIC_ICPR = NVIC_ICPR_CLRPEND(GPIO_PIN(30));
	
	/**
	 * Configura o modulo NVIC: seta prioriodade para IRQ30
	 * registrador NVIC_IPR7 mapeado em 0xe000e41cu (Tabela 3-7/p. 54 do Manual)
	 */
	NVIC_IPR7 |= NVIC_IP_PRI_30(prioridade << 6);

	return;
}



void GPIO_desativaSwitchesNVICInterrupt () {
//	NVIC_ICER = GPIO_PIN(30);            		
	NVIC_ICPR = NVIC_ICER_CLRENA(GPIO_PIN(30));    // Desabilita IRQ30
}

void GPIO_reativaSwitchesNVICInterrupt () {
//	NVIC_ISER = GPIO_PIN(30);            		// Reabilita IRQ30
	NVIC_ISER = NVIC_ISER_SETENA(GPIO_PIN(30));            		// Habilita interrupcao PORTA
}

void GPIO_desativaSwitches () {
	PORTA_PCR5 &= ~PORT_PCR_ISF_MASK | 
			PORT_PCR_IRQC(0b1111);   
	PORTA_PCR4 &= ~PORT_PCR_ISF_MASK | 
				PORT_PCR_IRQC(0b1111);
	PORTA_PCR12 &= ~PORT_PCR_ISF_MASK | 
				PORT_PCR_IRQC(0b1111);

}

void GPIO_reativaSwitches (uint8_t IRQA5_IRQC) {
	// Configura modo de interrupcao
	PORTA_PCR5 |= PORT_PCR_ISF_MASK |
			PORT_PCR_IRQC(IRQA5_IRQC );           	// modo de interrupcao
	PORTA_PCR4 |= PORT_PCR_ISF_MASK |
				PORT_PCR_IRQC(IRQA5_IRQC );           	// modo de interrupcao
	PORTA_PCR12 |= PORT_PCR_ISF_MASK |
				PORT_PCR_IRQC(IRQA5_IRQC );           	// modo de interrupcao
}
