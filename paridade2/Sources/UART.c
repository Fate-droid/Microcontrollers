/*!
 * @file UART.c
 * @brief Este modulo contem interface dos módulos UARTx.
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#include "derivative.h"
#include "UART.h"

UART_MemMapPtr UART[] = UART_BASE_PTRS ;

uint8_t UART0_configure(uint8_t fonte, UART0Config_type *config) {
	/*
	 * Ativa sinal de relogio do modulo UART0
	 */
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK ;
		
	SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;  // PLLFLLSEL=SIM_SOPT2[16]=0 (MCGFLLCLK)		
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC(0b11); // configurar a fonte de relógio (20.971520MHz)
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(fonte);	

	/*
	 * Desabilitar canais RX e TX para configuracoes
	 */ 
	UART0_C2 &= ~(UART0_C2_RE_MASK |           
			UART0_C2_TE_MASK);

	/*
	 * Configurar stop bits
	 */ 
	UART0_BDH &= ~(UART0_BDH_SBNS_MASK);
	UART0_BDH |= config->bdh_sbns << UART0_BDH_SBNS_SHIFT;

	/*!
	 * Setar baud rate
	 */ 
	config->sbr = (config->sbr & 0x1FFF);		// Mascarar os bits validos

	UART0_BDH &= ~UART0_BDH_SBR(0x1F);                             
	UART0_BDL &= ~UART0_BDL_SBR(0xFF);            
	UART0_BDH |= UART0_BDH_SBR(config->sbr >> 8);                             
	UART0_BDL |= UART0_BDL_SBR(config->sbr & 0x00FF);            

	/*
	 * Configurar o registrador C1
	 */ 
	UART0_C1 &= ~(UART0_C1_LOOPS_MASK |
			UART0_C1_DOZEEN_MASK |
			UART0_C1_RSRC_MASK |
			UART0_C1_M_MASK |                 
			UART0_C1_WAKE_MASK |
			UART0_C1_ILT_MASK |
			UART0_C1_PE_MASK |			
			UART0_C1_PT_MASK
	);

	UART0_C1 |= (config->c1_loops << UART0_C1_LOOPS_SHIFT |
			config->c1_dozeen << UART0_C1_DOZEEN_SHIFT |
			config->c1_rsrc << UART0_C1_RSRC_SHIFT |
			config->c1_m << UART0_C1_M_SHIFT |                 
			config->c1_wake << UART0_C1_WAKE_SHIFT |
			config->c1_ilt << UART0_C1_ILT_SHIFT |
			config->c1_pe << UART0_C1_PE_SHIFT |			
			config->c1_pt << UART0_C1_PT_SHIFT
	);

	/*
	 * Configurar o registrador C2
	 */ 
	UART0_C2 &= ~(UART0_C2_RWU_MASK |
			UART0_C2_SBK_MASK);

	UART0_C2 |= (config->c2_rwu << UART0_C2_RWU_SHIFT |
			config->c2_sbk << UART0_C2_SBK_SHIFT
	);
	
	/*
	 * Configurar o registrador C3
	 */ 
	UART0_C3 &= ~(UART0_C3_R8T9_MASK |
			UART0_C3_R9T8_MASK |
			UART0_C3_TXDIR_MASK |
			UART0_C3_TXINV_MASK 
	);

	UART0_C3 |= (config->c3_r8t9 << UART0_C3_R8T9_SHIFT |
			config->c3_r9t8 << UART0_C3_R9T8_SHIFT |
			config->c3_txdir << UART0_C3_TXDIR_SHIFT |
			config->c3_txinv << UART0_C3_TXINV_SHIFT 
	);

	/*
	 * Configurar o registrador C4
	 */ 
	UART0_C4 &= ~(UART0_C4_MAEN1_MASK |
			UART0_C4_MAEN2_MASK |
			UART0_C4_M10_MASK
	);

	UART0_C4 |= (config->c4_maen1 << UART0_C4_MAEN1_SHIFT |
			config->c4_maen2 << UART0_C4_MAEN2_SHIFT |
			config->c4_m10 << UART0_C4_M10_SHIFT
	);

	// Valores fora do intervalo 4 a 32 implicam em setar 16
	UART0_C4 |= (0b11111 << UART0_C4_OSR_SHIFT);
	UART0_C4 &= ((config->c4_osr << UART0_C4_OSR_SHIFT) | ~UART0_C4_OSR_MASK);

	/*
	 * Configurar o registrador C5		
	 */ 		
	UART0_C5 &= ~(UART0_C5_TDMAE_MASK |
			UART0_C5_RDMAE_MASK |
			UART0_C5_BOTHEDGE_MASK |
			UART0_C5_RESYNCDIS_MASK
	);

	UART0_C5 |= (config->c5_tdmae << UART0_C5_TDMAE_SHIFT |
			config->c5_rdmae << UART0_C5_RDMAE_SHIFT |
			config->c5_bothedge << UART0_C5_BOTHEDGE_SHIFT |
			config->c5_resyncdis << UART0_C5_RESYNCDIS_SHIFT
	);

	/*
	 * Ajustar amostragem conforme a taxa de superamostragem
	 */ 
	if ((config->c4_osr & 0b11111) > 2 && (config->c4_osr & 0b11111) < 7)
		UART0_C5 |= UART0_C5_BOTHEDGE_MASK;    

	/*!
	 * Configurar o registrador S2
	 */ 
	UART0_S2 &= ~(UART0_S2_MSBF_MASK |
			UART0_S2_RXINV_MASK |
			UART0_S2_RWUID_MASK |
			UART0_S2_BRK13_MASK |
			UART0_S2_LBKDE_MASK
	);

	UART0_S2 |= (config->s2_msbf << UART0_S2_MSBF_SHIFT |
			config->s2_rxinv << UART0_S2_RXINV_SHIFT |
			config->s2_rwuid << UART0_S2_RWUID_SHIFT |
			config->s2_brk13 << UART0_S2_BRK13_SHIFT |
			config->s2_lbkde << UART0_S2_LBKDE_SHIFT
	);

	/*!
	 * Resetar flaags de estado no registrador S1 (w1c)
	 */ 
	UART0_S1 |= (UART0_S1_IDLE_MASK |
			UART0_S1_OR_MASK |
			UART0_S1_NF_MASK |
			UART0_S1_FE_MASK |
			UART0_S1_PF_MASK
	);

	/*!
	 * Resetar flags de estado no registrador S2 (w1c)
	 */ 
	UART0_S2 |= (UART0_S2_LBKDIF_MASK |
			UART0_S2_RXEDGIF_MASK);

	/*!
	 * Habilitar os canais RX e TX
	 */
	UART0_C2 |= (UART0_C2_RE_MASK |         // habilita o canal receptor 
			UART0_C2_TE_MASK);	  			// habilita o canal transmissor  

	return 0;
}

uint8_t UART_configure(uint8_t m, uint8_t fonte, UARTConfig_type *config) {
	uint8_t x = m-1;

	/*
	 * Ativa sinal de relogio do modulo UARTx
	 */
	SIM_SCGC4 |= (SIM_SCGC4_UART1_MASK << x);
	
	SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;  // PLLFLLSEL=SIM_SOPT2[16]=0 (MCGFLLCLK)		
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC(0b11); // configurar a fonte de relógio (20.971520MHz)
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(fonte);	

	/*
	 * Desabilitar canais RX e TX para configuracoes
	 */ 
	UART[x]->C2 &= ~(UART_C2_RE_MASK |           
			UART_C2_TE_MASK);

	/*
	 * Configurar stop bits
	 */ 
	UART[x]->BDH &= ~(UART_BDH_SBNS_MASK);
	UART[x]->BDH |= config->bdh_sbns << UART_BDH_SBNS_SHIFT;

	/*!
	 * Setar baud rate
	 */
	config->sbr = (config->sbr & 0x1FFF);      // mascarar bits invalidos

	UART[x]->BDH &= ~UART_BDH_SBR(0x1F);                             
	UART[x]->BDL &= ~UART_BDL_SBR(0xFF);            
	UART[x]->BDH |= UART_BDH_SBR(config->sbr >> 8);                             
	UART[x]->BDL |= UART_BDL_SBR(config->sbr & 0x00FF);            

	/*
	 * Configurar o registrador C1
	 */ 
	UART[x]->C1 &= ~(UART_C1_LOOPS_MASK |
			UART_C1_UARTSWAI_MASK |
			UART_C1_RSRC_MASK |
			UART_C1_M_MASK |                 
			UART_C1_WAKE_MASK |
			UART_C1_ILT_MASK |
			UART_C1_PE_MASK |			
			UART_C1_PT_MASK
	);

	UART[x]->C1 |= (config->c1_loops << UART_C1_LOOPS_SHIFT |
			config->c1_uartswai << UART_C1_UARTSWAI_SHIFT |
			config->c1_rsrc << UART_C1_RSRC_SHIFT |
			config->c1_m << UART_C1_M_SHIFT |                 
			config->c1_wake << UART_C1_WAKE_SHIFT |
			config->c1_ilt << UART_C1_ILT_SHIFT |
			config->c1_pe << UART_C1_PE_SHIFT |			
			config->c1_pt << UART_C1_PT_SHIFT
	);

	/*
	 * Configurar o registrador C2
	 */ 
	UART[x]->C2 &= ~(UART_C2_RWU_MASK |
			UART_C2_SBK_MASK);

	UART[x]->C2 |= (config->c2_rwu << UART_C2_RWU_SHIFT |
			config->c2_sbk << UART_C2_SBK_SHIFT
	);

	/*
	 *  Configurar o registrador C3
	 */
	UART[x]->C3 &= ~(UART_C3_R8_MASK |
			UART_C3_T8_MASK |
			UART_C3_TXDIR_MASK |
			UART_C3_TXINV_MASK 
	);

	UART[x]->C3 |= (config->c3_r8 << UART_C3_R8_SHIFT |
			config->c3_t8 << UART_C3_T8_SHIFT |
			config->c3_txdir << UART_C3_TXDIR_SHIFT |
			config->c3_txinv << UART_C3_TXINV_SHIFT 
	);

	/*
	 * Configurar o registrador C4
	 */ 
	UART[x]->C4 &= ~(UART_C4_TDMAS_MASK |
			UART_C4_RDMAS_MASK
	);

	/*
	 * Configurar o registrador S2
	 */ 
	UART[x]->S2 &= ~(UART_S2_RXINV_MASK |
			UART_S2_RWUID_MASK |
			UART_S2_BRK13_MASK |
			UART_S2_LBKDE_MASK
	);

	UART[x]->S2 |= (config->s2_rxinv << UART_S2_RXINV_SHIFT |
			config->s2_rwuid << UART_S2_RWUID_SHIFT |
			config->s2_brk13 << UART_S2_BRK13_SHIFT |
			config->s2_lbkde << UART_S2_LBKDE_SHIFT
	);

	/*
	 * Resetar flags de estado no registrador S1 (w1c)
	 */ 
	UART[x]->S1 |= (UART_S1_IDLE_MASK |
			UART_S1_OR_MASK |
			UART_S1_NF_MASK |
			UART_S1_FE_MASK |
			UART_S1_PF_MASK
	);

	/*!
	 * Resetar flags de estado no registrador S2 (w1c)
	 */ 
	UART[x]->S2 |= (UART_S2_LBKDIF_MASK |
			UART_S2_RXEDGIF_MASK);

	/*!
	 * Habilitar os canais RX e TX
	 */
	UART[x]->C2 |= (UART_C2_RE_MASK |         		// habilita o canal receptor 
			UART_C2_TE_MASK);	  				// habilita o canal transmissor  
	
	return 0;
}

void UART0_initTerminal (UART0Config_type *config) {
	/*!
	 * Configura funcao RxTx dos pinos PTA1 e PTA2 conectados ao Terminal
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA_PCR1 |= PORT_PCR_MUX(0x2);        // UART0_RX
	PORTA_PCR2 |= PORT_PCR_MUX(0x2);        // UART0_TX

	UART0_configure (0b01, config); 		//MCGFLLCLK como fonte de clock
}

void UART_initH5Pin2 (UARTConfig_type *config) {
	/*!
	 * Configura funcao Tx do pino PTE22 conectado ao canal 2 do analisador logico
	 */	
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;
	
	PORTE_PCR22 |= PORT_PCR_MUX(0x4);  // UART2_TX

	UART_configure (2, 0b01, config);		//MCGFLLCLK como fonte de clock
}

void UART_habilitaIRQH5Pin2 (uint8_t prioridade) {
    /*! 
     * Ativa IRQ 14 (UART2) no NVIC: ativar, limpar pendencias e setar prioridade 1
     */
	NVIC_ISER |= 1 << (14) ;      		 ///< Habilitar IRQ14 (UART2)
	NVIC_ICPR |= 1 << (14) ;      		 ///< Limpar as pendencias em solicitacao de interrupcoes
	
	NVIC_IPR3 |= NVIC_IP_PRI_14(prioridade << 6); ///< 14/4->3 seta prioridade 		
}

void UART0_habilitaIRQTerminal (uint8_t prioridade) {
    /*! 
     * Ativa IRQ 12 (UART0) no NVIC: ativar, limpar pendencias e setar prioridade em 0
     */
	NVIC_ISER |= 1 << (12) ;      		 ///< Habilitar IRQ12 (UART0)
	NVIC_ICPR |= 1 << (12) ;      		 ///< Limpar as pendencias em solicitacao de interrupcoes
	NVIC_IPR3 |= NVIC_IP_PRI_12(prioridade << 6); ///< 12/4->3 seta prioridade 		
}

void UART0_habilitaInterruptRxTerminal() {
	UART0_C2 |= UART0_C2_RIE_MASK;	
}

void UART0_desabilitaInterruptRxTerminal() {
	UART0_C2 &= ~UART0_C2_RIE_MASK;	
}

void UART0_habilitaInterruptTxTerminal() {
	UART0_C2 |= UART0_C2_TIE_MASK;	
}

void UART0_desabilitaInterruptTxTerminal() {
	UART0_C2 &= ~UART0_C2_TIE_MASK;	
}
