/*!
 * @file timers.c
 * @brief Este modulo contem interface dos timers (SysTick, RTC, PIT, LPTMR0) do Kinetis MKL25
 * @author Wu Shin Ting
 * @date 01/03/2022
 */
#include "derivative.h"
#include "timers.h"

//====================================================
// SIM
//====================================================
/*!
 * @brief Seta o divisor de frequencia do sinal de barramento
 * @param[in] OUTDIV4 divisor do sinal MCGOUTCLK
 */
void SIM_setaOUTDIV4 (uint8_t OUTDIV4) {
	/*!
	 * Configurar a frequencia do clock de barramento atraves do divisor da frequencia 
	 * do sinal MCGOUTCLK  (clock de barramento = MCGOUTCLK/(OUTDIV1*OUTDIV4))
	 */
	SIM_CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4(0b111); //!< \li \l setar OUTDIV4
	SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(OUTDIV4); 	
}

//====================================================
// SYSTICK
//====================================================
void SysTick_init (uint32_t periodo) {

	SYST_RVR = SysTick_RVR_RELOAD(periodo);
	SYST_CVR = SysTick_CVR_CURRENT(0);          //!< \li \l reseta flag e recarga
	SYST_CSR |= (
				SysTick_CSR_CLKSOURCE_MASK 		//!< \li \l fonte de CLK: nucleo
				| SysTick_CSR_ENABLE_MASK     	//!< \li \l habilita SysTick
				);
}

void SysTick_ativaInterrupt () {
	SYST_CSR |= SysTick_CSR_TICKINT_MASK;    //!< \li \l ativa interrupcao do SysTick
}

void SysTick_desativaInterrupt () {
	SYST_CSR &= ~SysTick_CSR_TICKINT_MASK;    //!< \li \l desativa interrupcao do SysTick
}

//====================================================
// PIT
//====================================================
void PIT_initTimer0(uint32_t periodo) {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;            //!< \li \l ativar o sinal de relogio do PIT

	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK); 		//!< \li \l desativar o timer 
	PIT_TCTRL0 &= ~PIT_TCTRL_CHN_MASK;   		//!< \li \l timers nao encadeados
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	//!< \li \l carregar o valor de recarga

	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		//!< \li \l ativa timer 0

	PIT_MCR &= ~(PIT_MCR_FRZ_MASK |             //!< \li \l continua contagem no modo Debug
			PIT_MCR_MDIS_MASK );     			//!< \li \l habilita modulo PIT
}

void PIT_initSemAtivacaoTimer0(uint32_t periodo) {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;            //!< \li \l ativar o sinal de relogio do PIT

	PIT_TCTRL0 &= ~(PIT_TCTRL_TEN_MASK); 		//!< \li \l desativar o timer
	PIT_TCTRL0 &= ~PIT_TCTRL_CHN_MASK;   		//!< \li \l timers nao encadeados
	PIT_LDVAL0 = PIT_LDVAL_TSV(periodo);       	//!< \li \l carregar o valor de recarga

	PIT_TCTRL0 |= (PIT_TCTRL_TEN_MASK);  		//!< \li \l ativa timer 0

	PIT_MCR &= ~PIT_MCR_FRZ_MASK;               //!< \li \l continua contagem no modo Debug
	PIT_MCR |= PIT_MCR_MDIS_MASK;     			//!< \li \l desativa modulo PIT
}

void PIT_ativaTimer0() {
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;     			//ativa modulo PIT
}

void PIT_desativaTimer0() {
	PIT_MCR |= PIT_MCR_MDIS_MASK;     			// desativa modulo PIT
}

void PIT_ativaIRQ (uint8_t priority) {
    /*! 
     * Ativa IRQ 22 (PIT) no NVIC: ativar, limpar pendencias e setar prioridade 
     */
    NVIC_ISER |= 1 << (22);           	 // NVIC_ISER[22]=1 (habilita IRQ22)
    NVIC_ICPR |= 1 << (22);         	 // NVIC_ICPR[22]=1 (limpa as pend�ncias)
    NVIC_IPR5 |= NVIC_IP_PRI_22(priority << 6); // 22/4 -> 5

    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // ativar interrupcao em PIT
    
    return;
}

void PIT_desativaInterrupt () {
    PIT_TCTRL0  &= ~PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}

void PIT_reativaInterrupt () {
    PIT_TCTRL0  |= PIT_TCTRL_TIE_MASK;	  // desativar interrupcao em PIT

    return;
}
