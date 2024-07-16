/*
 * @file RTC.c
 * @brief Definicao dos prototipos relacionados com RTC
 * @date Jan 16, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "util.h"

void RTClpo_init()
{
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;         // ativar o sinal de relogio do RTC           
	
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b11);  // setar 1kHz LPO como fonte de ERCLK32K

	RTC_CR |= RTC_CR_SWR_MASK;			// resetar os registradores do RTC por software
										// limpar o contador RTC_TSR e flags RTC_SR_TOF e RTC_SR_TIF
	RTC_CR &= ~RTC_CR_SWR_MASK;			// resetar explicitamente SWR 
	
    RTC_TCR = RTC_TCR_CIR(0x00) |		// ajustar o erro de frequencia (ppm) em cada segundo
              RTC_TCR_TCR(0x00);        // 1s = 32.768 ticks (sem ajuste)
    
    RTC_TPR = 0; 			// resetar prescaler (2^15 - 1)
    RTC_TSR = 0;			// resetar o contador para que TIF seja baixada.
    
	RTC_SR |= RTC_SR_TCE_MASK;			// habilita o relogio
}

uint32_t RTClpo_getTime (uint32_t *seconds) {
	*seconds = (RTC_TSR*32768)/1000+(RTC_TPR/1000);  //!< reajustar a contagem para segundos
	
	return *seconds;
}

/**
 * @brief Setar o valor do contador de segundos do RTC
 * @param[in] seconds valor de tempo em segundos
 */
void RTClpo_setTime (uint32_t seconds) {
	RTC_SR &= ~RTC_SR_TCE_MASK;			//!< desabilita o modulo para atualizar prescaler e contador
	RTC_TPR = (seconds*1000)%32768;     //!< reajustar segundos para a frequencia 1kHz
	RTC_TSR = (seconds*1000)/32768;		
	RTC_SR |= RTC_SR_TCE_MASK;			//!< habilitar o modulo
}



void RTC_ativaAlarmIRQ (uint8_t priority) {
    /*!
     * Ativa IRQ 20 (Alarme) no NVIC: ativar, limpar pendencias e setar prioridade
     */
    NVIC_ISER |= (1 << 20);         	 // NVIC_ISER[20]=1 (habilita IRQ20)
    NVIC_ICPR |= (1 << 20);         	 // NVIC_ICPR[20]=1 (limpa as pendências)
    NVIC_IPR5 |= NVIC_IP_PRI_20(priority << 6); // 20/4 = 5			

    RTC_IER |= RTC_IER_TAIE_MASK; 		// ativa interrupcao em RTC

}

/******************************************************
 * 
 * Habilita pinos 1 e 2 de H5 para analisar os tempos em 1s e em um incremento de RTC_TSR
 * 
 ******************************************************/
void GPIO_initH5P1P2() {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;    		

	// Configure pinos 1, 2 com nivel 0
	// Funcao GPIO
	PORTE_PCR23 |= PORT_PCR_MUX(0x1);
	
	PORTE_PCR22 |= PORT_PCR_MUX(0x1); 
	
	// Sentido do sinal: saida
	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(22)) |
			GPIO_PDDR_PDD(GPIO_PIN(23));
	
	// Limpar os bits	
	GPIOE_PCOR = GPIO_PCOR_PTCO(GPIO_PIN(22)) |
			GPIO_PCOR_PTCO(GPIO_PIN(23)) ;	
}

void GPIO_toggleH5P1() {
	GPIOE_PTOR |= GPIO_PTOR_PTTO (GPIO_PIN(23));
}

void RTC_Seconds_IRQHandler() {
	GPIOE_PTOR |= GPIO_PTOR_PTTO (GPIO_PIN(22));
}


