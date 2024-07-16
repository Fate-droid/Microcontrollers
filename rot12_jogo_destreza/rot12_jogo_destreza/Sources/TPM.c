/*!
 * @file TPM.c
 * @brief Este modulo contem interface dos TPMx.
 * @author Fabio e Felipe
 * @date 08/11/2022
 */

#include "MKL25Z4.h"
#include "TPM.h"

static TPM_MemMapPtr TPM[] = TPM_BASE_PTRS;
//static PORT_MemMapPtr PORT[] = PORT_BASE_PTRS;

////////Duvida nessa funcao, perguntar pra professora
//retornar o valor do contador TPM[x] CNT
uint16_t TPM_LeCNT (uint8_t x){
	return TPM_CNT_REG(TPM[x]);
}

void TPM_ativaChIC (uint8_t x, uint8_t n, uint8_t mode){
	/*!
	 * Configura os canais para operar no modo Input Capture (00XX) ou Output Compare (01XX).
	 */
	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
			TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
			TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
			TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0

	TPM_CnSC_REG(TPM[x],n) |= (mode << 2);      // rising/falling/both edges
	
	TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
	TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1	

}
void TPM_ativaChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor){
	/*!
		 * Configura os canais para operar no modo Output Compare mode (0b01, 0b10, 0b11)  
		 */
		TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPM0_C2SC[5]=0
				TPM_CnSC_MSA_MASK |         // MSA=TPM0_C2SC[4]=0
				TPM_CnSC_ELSB_MASK |        // ELSB=TPM0_C2SC[3]=0
				TPM_CnSC_ELSA_MASK );       // ELSA=TPM0_C2SC[2]=0
		/*!
		 * Setar o valor em TPMx_CnV
		 */
		TPM_CnV_REG(TPM[x],n) = TPM_CnV_VAL(valor); 

		TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSA_MASK |   // MSA=TPM0_C2SC[4]=1
					(mode << 2));      	// sem uso/toggle/clear/set output
		
		TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
		TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1	
}

void TPM_desativaCh (uint8_t x, uint8_t n) {
	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPM0_C2SC[5]=0
			TPM_CnSC_MSA_MASK |         // MSA=TPM0_C2SC[4]=0
			TPM_CnSC_ELSB_MASK |        // ELSB=TPM0_C2SC[3]=0
			TPM_CnSC_ELSA_MASK );       // ELSA=TPM0_C2SC[2]=0
	
	TPM_CnSC_REG(TPM[x],n) &= ~TPM_CnSC_CHIE_MASK; 	    // desabilitar interrupcao do canal 1	

}

void TPM_ativaChInterrupt (uint8_t x, uint8_t n){
	TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
	TPM_CnSC_REG(TPM[x],n) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal n	
}

void TPM_desativaChInterrupt (uint8_t x, uint8_t n){
	TPM_CnSC_REG(TPM[x],n)&= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal n de TPMx		
}
//
//conectar as
//botoeiras NMI, IRQA5 e IRQA12 nos canais TPM0_CH1, TPM0_CH2 e TPM1_CH0 cujos
//contadores são configurados com TPMx_MOD = periodo e TPMx_SC_PS = ps
void TPM_initSwitchesIC (uint32_t periodo, uint8_t ps){

	TPM_init (0, periodo, ps);					//configura TPM0
	TPM_init (1, periodo, ps);					// configura TPM1
	/*!
	 * Configura os pinos: PTA4 (entrada), PTA5 E PTA12
	 */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;			// habilita clock de PORTA
	PORTA_PCR4 &= ~PORT_PCR_MUX(0x4);			// PTA4 em TPM0_CH1  
	PORTA_PCR5 &= PORT_PCR_MUX(0x0);			// PTA5 em TPM0_CH2
	PORTA_PCR5 |= PORT_PCR_MUX(0x3);
	PORTA_PCR12 &= PORT_PCR_MUX(0x0);			// PTA1 em TPM1_CH0
	PORTA_PCR12 |= PORT_PCR_MUX(0x3);
	
	TPM_desativaCh (1, 0);
	TPM_desativaCh (0, 2);   // "" botoeira IRQA5
	TPM_desativaCh (0, 1); // "" botoeeira NMI
	
}

//: conectar os
//leds R e G nos canais TPM2_CH0 e TPM2_CH1 cujo contador é configurado com
//TPMx_MOD=periodo e TPMx_SC_PS=ps.
void TPM_initPTB18PTB19OC (uint32_t periodo, uint8_t ps){
	TPM_init (2, periodo, ps);				// configura TPM2
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;      // habilita clock de PORTB
	PORTB_PCR18 &= PORT_PCR_MUX(0x0);
	PORTB_PCR18 |= PORT_PCR_MUX(0x3);		// PTB18 em TPM2_CH0  
	PORTB_PCR19 &= PORT_PCR_MUX(0x0);		
	PORTB_PCR19 |= PORT_PCR_MUX(0x3);		// PTB19 em  TPM2_CH1
	
//	TPM_initChOC(2,0, 0b00, 0b00);
//	TPM_initChOC(2,1, 0b00, 0b00);
	TPM_desativaCh (2, 0);       //Desativar LED R
	TPM_desativaCh (2, 1);       //Desativar LED G
	
}
//	: configurar NVIC para
//habilitar a interrupção (Input Capture) dos canais em que as botoeiras são conectadas com os canais
//desativados
void TPM_habilitaSwitchesInterrupt (char priority){
    NVIC_ISER |= 1 << (17);              // NVIC_ISER[17]=1 (habilita IRQ17)
    NVIC_ICPR |= 1 << (17);              // NVIC_ICPR[17]=1 (limpa as pendÃªncias)
    NVIC_IPR4 |= NVIC_IP_PRI_17(priority << 6);    
    
    NVIC_ISER |= 1 << (18);              // NVIC_ISER[18]=1 (habilita IRQ18)
    NVIC_ICPR |= 1 << (18);              // NVIC_ICPR[18]=1 (limpa as pendÃªncias)
    NVIC_IPR4 |= NVIC_IP_PRI_18(priority << 6);    
	/*!
	 * Habilitar o evento de interrupcao
	 */ 
//	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1	
//
//	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 2	
//	
//	TPM_CnSC_REG(TPM[1],0) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[1],0) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 0 de TPM1	
	
	
}
//configurar
//NVIC para habilitar a interrupção (Output Compare) dos canais em que os leds R e G são
//conectados com os canais desativados.
void TPM_habilitTPM2CH0TPM2CH1Interrupt (char priority){
	NVIC_ISER |= 1 << (19);              // NVIC_ISER[17]=1 (habilita IRQ17)
	NVIC_ICPR |= 1 << (19);              // NVIC_ICPR[17]=1 (limpa as pendÃªncias)
	NVIC_IPR4 |= NVIC_IP_PRI_19(priority << 6); 
	
	/*!
	 * Habilitar o evento de interrupcao
	 */ 
//	TPM_CnSC_REG(TPM[2],0) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[2],0) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 0	
//
//	TPM_CnSC_REG(TPM[2],1) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[2],1) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1

}


//
////ativar o canal de NMI para IC (borda de
////descida) e a interrupção do canal.
//void TPM_ativaSwitchNMIInterrupt (){
//	TPM_initChIC (0, 1, 0b10);					// configura Ch1, servido pelo PTA4, para IC
//	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1	
//}
//
//
////: desativar o canal de NMI
////(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
//void TPM_desativaSwitchNMIInterrupt (){
//	TPM_CnSC_REG(TPM[0],1) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//			TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//			TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//			TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//	
//	TPM0_C1SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 1		
//}
//
//////: ativar o canal de IRQA5 para IC (borda de
//////descida) e a interrupção do canal.
//void TPM_ativaSwitchIRQA5Interrupt (){
//	TPM_initChIC (0, 2, 0b10);					// configura Ch2, servido pelo PTA5, para IC
//	TPM0_C2SC |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 1		
//}
//
//
//////desativar o canal de IRQA5
//////(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
//void TPM_desativaSwitchIRQA5Interrupt (){
//	TPM_CnSC_REG(TPM[0],2) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//				TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//				TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//				TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//		
//	TPM0_C2SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 1	
//}
//
//
//////ativar o canal de IRQA12 para IC (borda
//////de descida) e a interrupção do canal.
////
////
//void TPM_ativaSwitchIRQA12Interrupt (){
//	TPM_initChIC (1, 0, 0b10);					// configura Ch servido pelo PTA1, para IC
//	TPM1_C0SC |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[1],0) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 1		
//	
//}
//////: desativar o canal de IRQA12
//////(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
//void TPM_desativaSwitchIRQA12Interrupt (){
//	TPM_CnSC_REG(TPM[1],0) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//					TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//					TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//					TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//			
//	TPM1_C0SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 0
//}
//
//
//////: ativar o canal de led R para OC (Clear Output on
//////match) e a interrupção do canal. (canal do LED R : TPM2_CH0)
////
//void TPM_ativaLedRInterrupt (){
//	TPM_initChOC (2, 0, 0b10,0);
//	
//	//ativa interrupcao do canal
//	TPM2_C0SC |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[2],0) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 1		
//}
//
//
//////desativar o canal de led R
//////(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
////
//void TPM_desativaLedRInterrupt (){
//	TPM_CnSC_REG(TPM[2],0) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//					TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//					TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//					TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//			
//	TPM2_C0SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 0
//}
////
//////ativar o canal de led G para OC,
//////cujo modo pode ser Clear Output on match (0b10) ou Set Output on match (0b11) e a interrupção
//////do canal. (canal do LED G: TPM2_CH1)
////
//void TPM_ativaLedGInterrupt (uint8_t modo){
//	TPM_initChOC (2, 1, modo,0);
//	
//	//ativa interrupcao do canal
//	TPM2_C1SC |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
//	TPM_CnSC_REG(TPM[2],1) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 1		
//}
////
//////desativar o canal de led G
//////(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
//void TPM_desativaLedGInterrupt (){
//	TPM_CnSC_REG(TPM[2],1) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//					TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//					TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//					TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//			
//	TPM2_C1SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 0
//}
//
//
////
////
////
////void TPM_initH5Pin12EPWM (uint16_t periodo, uint8_t ps, uint8_t percentagem, booleano_type mode) {
////	
////	TPM_init (2, periodo, ps);
////	
////	/*!
////	 * Configura a funcao dos pinos em TPM
////	 */
////	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; 			///< \li \l habilita clock de PORTE                           	
////	PORT_PCR_REG(PORT[4],22) |= (PORT_PCR_ISF_MASK |			///< \li \l PTE22 em TPM2_CH0 (H5, pino 2)
////				  PORT_PCR_MUX(0x3) );       
////	PORT_PCR_REG(PORT[4],23) |= (PORT_PCR_ISF_MASK |			///< \li \l PTE23 em TPM2_CH1 (H5, pino 1)
////				  PORT_PCR_MUX(0x3) );      
////
////	/*!
////	 * Configura os canais para gerarem EPWM (edge-aligned PWM): 0b1010 (High) ou 0b10X1 (Low) 
////	 * Configura o valor de match inicial
////	 */
////	TPM_initChEPWM (2, 0, percentagem, mode);
////	TPM_initChEPWM (2, 1, percentagem, mode);
////
////}
////
////void TPM_atualizaDutyCycleH5Pin2 (uint8_t percentagem)
////{
////	TPM_CnV_REG(TPM[2],0) = TPM_CnV_VAL(percentagem*0.01*(TPM2_MOD+1));
////}
////
////void TPM_atualizaDutyCycleH5Pin1 (uint8_t percentagem)
////{
////
////	TPM_CnV_REG(TPM[2],1) = TPM_CnV_VAL(percentagem*0.01*(TPM[2]->MOD+1));
////}
//
void TPM_init (uint8_t x, uint16_t periodo, uint8_t ps) {
	
	SIM_SCGC6 |= GPIO_PIN(24+x); 	///< \li \l habilita clock de TPMx                           	

	/**
	 * Fonte de CLK de TPM: TPMSRC=SIM_SOPT2[25:24]=01 (MCGFLLCLK|MCGPLLCLK/2)
	 * PLLFLLSEL=SIM_SOPT2[16]=0 (MCGFLLCLK) 
	 */
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b01);     ///< \li \l seleciona fonte de clock TPM
	SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;  	

	TPM[x]->SC &= ~TPM_SC_CMOD(0x3);		///< desabilita contador

	TPM[x]->SC &= ~(TPM_SC_TOIE_MASK |		///< \li \l desabilita interrupcao "overflow"
				TPM_SC_CPWMS_MASK); 	///< \li \l modo de contagem crescente

	TPM[x]->MOD &= TPM_MOD_MOD(periodo);    	///< \li \l seta contagem maxima
	
	TPM[x]->SC &= ~TPM_SC_PS(0b111);       ///< \li \l seta prescaler 
	TPM[x]->SC |= TPM_SC_PS(ps);         	

	TPM[x]->CNT |= TPM_CNT_COUNT(0x0)  ;   ///< \li \l reseta a contagem com base em LPTPM
	
	TPM[x]->SC |= TPM_SC_CMOD(0x1);		///< \li \l habilita contador 
}
//
////void TPM_initChEPWM (uint8_t x, uint8_t n, uint8_t percentagem, booleano_type mode) {
////	/*!
////	 * Configura os canais para gerarem EPWM (edge-aligned PWM): 0b1010 (High) ou 0b10X1 (Low) 
////	 */
////	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
////			TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
////			TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
////			TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
////
////	if (mode == ON) { //High-true
////		TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSB_MASK |       // MSB=TPMx_CnSC[5]=1
////				TPM_CnSC_ELSB_MASK );     		// ELSB=TPMx_CnSC[3]=1
////	} else if (mode == OFF) { //Low-true
////		TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSB_MASK |       // MSB=TPMx_CnSC[5]=1
////				TPM_CnSC_ELSA_MASK );     		// ELSB=TPMx_CnSC[2]=1
////	}
////	
////	/*!
////	 * Configura o valor de match inicial
////	 */
////	TPM_CnV_REG(TPM[x],n) = TPM_CnV_VAL(percentagem*0.01*(TPM[x]->MOD+1)); // TPMx_CnV	
////}
////
////void TPM_atualizaPolaridadeEPWM (uint8_t x, uint8_t n, booleano_type mode)
////{
////	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |      ///< desabilita canal TPM2_CH1
////			TPM_CnSC_MSA_MASK |         
////			TPM_CnSC_ELSB_MASK |        
////			TPM_CnSC_ELSA_MASK );       
////
////	if (mode == ON) 						///< reabilita o canal no modo HIGH
////		TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSB_MASK |      
////				TPM_CnSC_ELSB_MASK );     
////	else if (mode == OFF) 					///< reabilita o canal no modo LOW
////		TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSB_MASK |       
////				TPM_CnSC_ELSA_MASK );     		
////}
////
////void TPM_initSwitchNMIChannelTPM0CH2 (uint32_t periodo, uint8_t ps) {
////
////	TPM_init (0, periodo, ps);					//configura TPM0
////
////	/*!
////	 * Configura os pinos: PTA4 (entrada)
////	 */
////	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;			// habilita clock de PORTA
////	PORTA_PCR4 &= ~PORT_PCR_MUX(0x4);			// PTA4 em TPM0_CH1     
////
////	TPM_initChIC (0, 1, 0b10);					// configura Ch1, servido pelo PTA4, para IC
////	TPM_initChOC (0, 2, 0b10, 0);				// configura Ch2, para OC
////}
////
////void TPM_initH5Pin3OC (uint32_t periodo, uint8_t ps) {
////
////	TPM_init (1, periodo, ps);					//configura TPM0
////
////	/*!
////	 * Configura os pinos: PTE21 (saida)
////	 */
////	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;			// habilita clock de PORTA
////	PORTE_PCR21 |= PORT_PCR_MUX(0x3);			// PTA4 em TPM0_CH1     
////
////	TPM_initChOC (1, 1, 0b10, 0);				// clear o canal 1,1 no reset
////}
////
////void TPM_initChIC (uint8_t x, uint8_t n, uint8_t mode) {
////	/*!
////	 * Configura os canais para operar no modo Input Capute mode (0b01, 0b10 e 0b11)
////	 */
////	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
////			TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
////			TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
////			TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
////
////	TPM_CnSC_REG(TPM[x],n) |= (mode << 2);      // rising/falling/both edges
////}
//////
////void TPM_initChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor) {
////	/*!
////	 * Configura os canais para operar no modo Output Compare mode (0b01, 0b10, 0b11)  
////	 */
////	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPM0_C2SC[5]=0
////			TPM_CnSC_MSA_MASK |         // MSA=TPM0_C2SC[4]=0
////			TPM_CnSC_ELSB_MASK |        // ELSB=TPM0_C2SC[3]=0
////			TPM_CnSC_ELSA_MASK );       // ELSA=TPM0_C2SC[2]=0
////	/*!
////	 * Setar o valor em TPMx_CnV
////	 */
////	TPM_CnV_REG(TPM[x],n) = TPM_CnV_VAL(valor); 
////
////	TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSA_MASK |   // MSA=TPM0_C2SC[4]=1
////				(mode << 2));      	// sem uso/toggle/clear/set output
////}
//
//void TPM_initChIC (uint8_t x, uint8_t n, uint8_t mode) {
//	/*!
//	 * Configura os canais para operar no modo Input Capute mode (0b01, 0b10 e 0b11)
//	 */
//	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPMx_CnSC[5]=0
//			TPM_CnSC_MSA_MASK |         // MSA=TPMx_CnSC[4]=0
//			TPM_CnSC_ELSB_MASK |        // ELSB=TPMx_CnSC[3]=0
//			TPM_CnSC_ELSA_MASK );       // ELSA=TPMx_CnSC[2]=0
//
//	TPM_CnSC_REG(TPM[x],n) |= (mode << 2);      // rising/falling/both edges
//}
//
//void TPM_initChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor) {
//	/*!
//	 * Configura os canais para operar no modo Output Compare mode (0b01, 0b10, 0b11)  
//	 */
//	TPM_CnSC_REG(TPM[x],n) &= ~(TPM_CnSC_MSB_MASK |  // MSB=TPM0_C2SC[5]=0
//			TPM_CnSC_MSA_MASK |         // MSA=TPM0_C2SC[4]=0
//			TPM_CnSC_ELSB_MASK |        // ELSB=TPM0_C2SC[3]=0
//			TPM_CnSC_ELSA_MASK );       // ELSA=TPM0_C2SC[2]=0
//	/*!
//	 * Setar o valor em TPMx_CnV
//	 */
//	TPM_CnV_REG(TPM[x],n) = TPM_CnV_VAL(valor); 
//
//	TPM_CnSC_REG(TPM[x],n) |= (TPM_CnSC_MSA_MASK |   // MSA=TPM0_C2SC[4]=1
//				(mode << 2));      	// sem uso/toggle/clear/set output
//}

//
//
////
////void TPM_habilitaSwitchNMIChannelTPM0CH2Interrupt (char priority) {	
////    NVIC_ISER |= 1 << (17);              // NVIC_ISER[17]=1 (habilita IRQ17)
////    NVIC_ICPR |= 1 << (17);              // NVIC_ICPR[17]=1 (limpa as pendÃªncias)
////    NVIC_IPR4 |= NVIC_IP_PRI_17(priority << 6);    
////    
////	/*!
////	 * Habilitar o evento de interrupcao
////	 */ 
////	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
////	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 1	
////
////	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
////	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do canal 2	
////}
////
////void TPM_ativaSwitchNMIInterrupt () {
////	TPM0_C1SC |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
////	TPM_CnSC_REG(TPM[0],1) |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 1		
////}
////
////void TPM_desativaSwitchNMIInterrupt () {
////	TPM0_C1SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 1		
////}
////
////void TPM_ativaChannelTPM0CH2Interrupt () {
////	TPM_CnSC_REG(TPM[0],2) |= TPM_CnSC_CHF_MASK;      	// limpar pendencias 	
////	TPM0_C2SC |= TPM_CnSC_CHIE_MASK; 	    // ativa interrupcao do canal 2		
////}
////
////void TPM_desativaChannelTPM0CH2Interrupt () {
////	TPM0_C2SC &= ~TPM_CnSC_CHIE_MASK; 	    // desativa interrupcao do canal 2		
////}
//
