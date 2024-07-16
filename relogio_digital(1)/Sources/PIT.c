/*
 * @file PIT.c
 * @brief Definicao das funcoes relacionadas com o modulo PIT
 * @date Jan 16, 2023
 * @author Wu Shin Ting
 */

#include "derivative.h"
#include "util.h"

void PIT_initTimer0(uint32_t periodo, uint8_t prioridade) {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;            // ativar o sinal de relogio do PIT

	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK); 		// desativar o timer 
	PIT_TCTRL0 &= ~PIT_TCTRL_CHN_MASK;   		// timers nao encadeados
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	// carregar o valor de recarga

	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		// ativa timer 0

    // Ativa IRQ 22 (PIT) no NVIC: ativar, limpar pendencias e setar prioridade 
    NVIC_ISER = (1 << 22);           	 // NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR = (1 << 22);         	 // NVIC_ICPR[22]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_22(prioridade << 6); // 22/4 -> 5

    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // ativar interrupcao em PIT

	PIT_MCR &= ~(PIT_MCR_FRZ_MASK |             // continua contagem no modo Debug
			PIT_MCR_MDIS_MASK );     			// habilita modulo PIT	
}

void PIT_ativaNVICIRQ (uint8_t prioridade) { 
    // Ativa IRQ 22 (PIT) no NVIC: ativar, limpar pendencias e setar prioridade 
    NVIC_ISER = (1 << 22);           	 // NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR = (1 << 22);         	 // NVIC_ICPR[22]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_22(prioridade << 6); // 22/4 -> 5

    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // ativar interrupcao em PIT
    
    return;
}

void PIT_desabilitaIRQ () {
    PIT_TCTRL0  &= ~PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}

void PIT_reabilitaIRQ () {
    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}

void PIT_setaLDVAL (uint32_t periodo) {
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	// carregar o valor de recarga

    return;
}

void PIT_desativaTimer0 () {
	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK);  		// desativa timer 0

    return;
}

void PIT_ativaTimer0() {
	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		// ativa timer 0

	return;
}

/******************************************************
 * 
 * Habilita pinos 3 e 4 de H5 para analisar os sinais 
 * 
 ******************************************************/
void GPIO_initH5P3P4() {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;    		

	// Configure pinos 3 e 4 com nivel 0
	// Funcao GPIO
	PORTE_PCR20 |= PORT_PCR_MUX(0x1);  
	PORTE_PCR21 |= PORT_PCR_MUX(0x1);  

	// Sentido do sinal: saida
	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(20)) |
			GPIO_PDDR_PDD(GPIO_PIN(21));
	
	// Limpar o bit	
	GPIOE_PCOR = GPIO_PDDR_PDD(GPIO_PIN(20)) |
			GPIO_PCOR_PTCO(GPIO_PIN(21));
}



