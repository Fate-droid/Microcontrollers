/*!
 * @file TPM.c
 * @brief Este modulo contem interface dos módulos TPMx.
 * @author Wu Shin Ting
 * @date 26/01/2023
 */

//#include "MKL25Z4.h"
#include "TPM.h"
#include "derivative.h"
#include "stdint.h"
static TPM_MemMapPtr TPM[] = TPM_BASE_PTRS;

void TPM_habilitaSwitchesInterrupt (char priority){
    NVIC_ISER |= 1 << (17);              // NVIC_ISER[17]=1 (habilita IRQ17)
    NVIC_ICPR |= 1 << (17);              // NVIC_ICPR[17]=1 (limpa as pendÃªncias)
    NVIC_IPR4 |= NVIC_IP_PRI_17(priority << 6);    
    
    NVIC_ISER |= 1 << (18);              // NVIC_ISER[18]=1 (habilita IRQ18)
    NVIC_ICPR |= 1 << (18);              // NVIC_ICPR[18]=1 (limpa as pendÃªncias)
    NVIC_IPR4 |= NVIC_IP_PRI_18(priority << 6);    
}




void TPM0TPM1_PTA12PTA13PTC2PTC3_config_basica() {
	
	/*
	 * Habilita os sinais de relógio para TPM0 e TPM1
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM0_MASK| 				
			SIM_SCGC6_TPM1_MASK);

	/*
	 * Habilita os sinais de relógio para TPM1 e TPM2
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK| 				
			SIM_SCGC6_TPM2_MASK);

		/*
	 * Configura pinos PTA12 e PTA13, PTC2 e PTC3
	 */ 
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK|				//habilita sinais de relogio
			SIM_SCGC5_PORTC_MASK);

	PORTA_PCR12 |= (PORT_PCR_ISF_MASK |				//TPM1_CH0
			PORT_PCR_MUX(0x3));
	PORTA_PCR13 |= (PORT_PCR_ISF_MASK |				//TPM1_CH1
			PORT_PCR_MUX(0x3));
	PORTC_PCR2 |= (PORT_PCR_ISF_MASK |				//TPM0_CH1
			PORT_PCR_MUX(0x4)| 
			PORT_PCR_DSE_MASK);						//drive strength enable
													//e.g. 18 mA vs. 5 mA @ > 2.7 V,
	PORTC_PCR3 = (PORT_PCR_ISF_MASK |				//TPM0_CH2
			PORT_PCR_MUX(0x4)| 
			PORT_PCR_DSE_MASK);
	
	return;
}

void TPM1TPM2_PTE20PTE21PTE22PTE23_config_basica() {
	
	/*
	 * Habilita os sinais de relógio para TPM1 e TPM2
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK| 				
			SIM_SCGC6_TPM2_MASK);

	/*
	 * Configura pinos PTE20 e PTE21, PTE22 e PTE23
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;				//habilita sinais de relogio de PTE 

	PORTE_PCR20 |= (PORT_PCR_ISF_MASK |				//TPM1_CH0
			PORT_PCR_MUX(0x3));
	PORTE_PCR21 |= (PORT_PCR_ISF_MASK |				//TPM1_CH1
			PORT_PCR_MUX(0x3));
	PORTE_PCR22 |= (PORT_PCR_ISF_MASK |				//TPM2_CH0
			PORT_PCR_MUX(0x3)| 
			PORT_PCR_DSE_MASK);						//drive strength enable
													//e.g. 18 mA vs. 5 mA @ > 2.7 V,
	PORTE_PCR23 = (PORT_PCR_ISF_MASK |				//TPM2_CH1
			PORT_PCR_MUX(0x3)| 
			PORT_PCR_DSE_MASK);
	
	return;
}

void TPM0DMA_PTC3_config_basica() {
	
	/*
	 * Habilita os sinais de relógio para TPM0
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM0_MASK |
			SIM_SCGC6_DMAMUX_MASK |
			SIM_SCGC6_FTF_MASK);

	/*
	 * Habilitar DMA
	 */
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
	
	/*
	 * Configura pinos PTA4 PTC3
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;				//habilita sinais de relogio de PTC

	PORTC_PCR3 = (PORT_PCR_ISF_MASK |				//TPM0_CH2
			PORT_PCR_MUX(0x4)| 
			PORT_PCR_DSE_MASK);						//acionamento maior na corrente de saida
	
	return;
}

void TPM1DMA_PTE21_config_basica() {
	
	/*
	 * Habilita os sinais de relógio para TPM1
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK |
			SIM_SCGC6_DMAMUX_MASK |			// habilitar o circuito de roteamento das fontes para os canais DMA
			SIM_SCGC6_FTF_MASK);			// habilitar acessos de leitura a flash

	/*
	 * Habilitar DMA
	 */
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;	   // habiliter DMA
	
	/*
	 * Configura pinos PTE21
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;				//habilita sinais de relogio

	PORTE_PCR21 = (PORT_PCR_ISF_MASK |				//TPM1_CH1
			PORT_PCR_MUX(0x3)| 
			PORT_PCR_DSE_MASK);						//acionamento maior na corrente de saida
	
	return;
}

void TPM1TPM2_PTB0PTB18PTB19_config_basica(){
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	SIM_SCGC5 |= (SIM_SCGC5_PORTB_MASK);
	PORTB_PCR0 = (PORT_PCR_ISF_MASK |				//TPM1_CH0
				PORT_PCR_MUX(0x3)| 
				PORT_PCR_DSE_MASK);	
	PORTB_PCR18 = (PORT_PCR_ISF_MASK |				//TPM2_CH0
				PORT_PCR_MUX(0x3)| 
				PORT_PCR_DSE_MASK);	
	
	PORTB_PCR19 = (PORT_PCR_ISF_MASK |				//TPM2_CH1
				PORT_PCR_MUX(0x3)| 
				PORT_PCR_DSE_MASK);	
	
}

void TPM0TPM1_PTAPTE_config_basica () {
	/*
	 * Habilita os sinais de relógio para TPM0 e TPM1
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM0_MASK |
		SIM_SCGC6_TPM1_MASK);

	/*
	 * Habilitar os sinais de relogio para PORTA e PORTE
	 */
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK 			// habilita clock de PORTA
		|SIM_SCGC5_PORTE_MASK);				// habilita clock de PORTE

	/*
	 * Configurar pinos
	 */	
	PORTA_PCR4 &= ~PORT_PCR_MUX(0x4);			// PTA4 em TPM0_CH1 
	//PORTA_PCR4 |= PORT_PCR_MUX(0x3);			// PTA4 em TPM0_CH1
	PORTA_PCR12 |= PORT_PCR_MUX(0x3);			// PTA12 em TPM1_CH0     
	PORTE_PCR21 |= PORT_PCR_MUX(0x3);			// PTE21 em TPM1_CH1
}

void TPM_config_especifica (uint8_t x, uint16_t mod, uint8_t trigger, uint8_t crot, uint8_t csoo, uint8_t csot,
		uint8_t dma, uint8_t cpwms, uint8_t ps) {

	/*
	 * Configurar disparo/recarga do contador CNT
	 */
	uint8_t temp = trigger & 0b1111;
	
	if (temp != 0b0010 && temp != 0b0011 && temp != 0b0110 &&
			temp != 0b0111 && temp != 0b1011 && temp != 0b1111) {
		TPM[x]->CONF &= ~((0b1111<<24)|(0b111<<16));
		TPM[x]->CONF |= TPM_CONF_TRGSEL(trigger);		//Tabela 3-38/p. 86
		if (crot) TPM[x]->CONF |= TPM_CONF_CROT_MASK;           //Resetar CNT no disparo
		if (csoo) TPM[x]->CONF |= TPM_CONF_CSOO_MASK;			//CNT para em TOF 
		if (csot) TPM[x]->CONF |= TPM_CONF_CSOT_MASK;			//Ativar contador no disparo
	}
	/*
	 * Resetar o contador fazendo um acesso de escrita
	 */
	TPM[x]->CNT = 0;
	
	/*
	 * Configurar periodo do contador T = PS*MOD/freq.
	 */
	TPM[x]->SC &= ~TPM_SC_PS(0b111);
	TPM[x]->SC |= TPM_SC_PS(ps);
	if (dma) TPM[x]->SC |= TPM_SC_DMA_MASK;
	if (cpwms) TPM[x]->SC |= TPM_SC_CPWMS_MASK;
	
	/**
	 * Configurar a contagem maxima
	 */
	TPM[x]->MOD = TPM_MOD_MOD(mod);
	
	TPM[x]->SC |= TPM_SC_CMOD(1);			// ativar o contador LPTPM
	return;
}

void TPM_CH_config_especifica (uint8_t x, uint8_t n, uint8_t MS_ELS, uint16_t valor) {
    /*
     * Configurar o modo de operacao do canal
     */
	TPM[x]->CONTROLS[n].CnSC &= ~(0b1111<<2);
	TPM[x]->CONTROLS[n].CnSC |= (MS_ELS<<2);
	
	/*
	 * Inicializar com valor 0 no contador
	 */
	TPM[x]->CONTROLS[n].CnV = TPM_CnV_VAL(valor);	
			
	return;
}

void TPM_habilitaNVICIRQ(uint8_t irq, uint8_t prioridade) {
	/**
	 * Configura o modulo NVIC: habilita IRQ irq
	 */
	NVIC_ISER = NVIC_ISER_SETENA(1<<irq);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ irq
	 */
	NVIC_ICPR = NVIC_ICPR_CLRPEND(1<<irq);	

	/**
	 * Configura o modulo NVIC: seta prioriodade 3 para IRQ
	 * registrador NVIC_IPRn n = irq/4(Tabela 3-7/p. 53 do Manual)
	 * campo dentro do registrador: (irq%4)*8
	 */
	NVIC_IP_REG(NVIC_BASE_PTR,irq/4) |= (prioridade<<6) << (irq%4*8); //(Secao 3.3.2.1/p. 53 do Manual)
	
	return;
}

void TPM_habilitaInterrupTOF(uint8_t x) {
	TPM[x]->SC |= (TPM_SC_TOF_MASK |	// resetar flag
			TPM_SC_TOIE_MASK);			// habilitar a interrupcao TOF
}

void TPM_desabilitaInterrupTOF(uint8_t x) {
	TPM[x]->SC &= ~(TPM_SC_TOIE_MASK);
}

void TPM_habilitaInterrupCH(uint8_t x, uint8_t n) {
	TPM[x]->CONTROLS[n].CnSC |= (TPM_CnSC_CHF_MASK |	// resetar flag
			 TPM_CnSC_CHIE_MASK);			// habilitar a interrupcao do canal
}

void TPM_setaCnV (uint8_t x, uint8_t n, uint16_t valor) {
	TPM[x]->CONTROLS[n].CnV = TPM_CnV_VAL(valor);
}

uint16_t TPMm_leCNT(uint8_t x){
	return TPM[x]->CNT;
	//return TPM_CNT_REG (TPM[x]);
	
}

void TPM_desativaCh (uint8_t x, uint8_t n) {
	TPM_CH_config_especifica (x, n, 0b0000, 0);
	TPM[x]->CONTROLS[n].CnSC &=~(TPM_CnSC_CHIE_MASK);			// desabilitar a interrupcao do cana
	
//	TPM_CnSC_REG(TPM[x],n) &= ~TPM_CnSC_CHIE_MASK; 	    // desabilitar interrupcao do canal 1	

}


