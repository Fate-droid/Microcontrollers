/*!
 * @file UART.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo UART.c
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#ifndef UART_H_
#define UART_H_

/*!
 * @struct _UART0Configuration_tag
 */
typedef struct _UART0Configuration_tag {
	uint8_t	bdh_sbns;		///< selecionar quantidade de stop bits (0 = 1; 1 = 2)
	uint16_t sbr;		
	uint8_t c1_loops;       ///< operacao em loop (normal = 0)
	uint8_t c1_dozeen;		///< habilitar espera (doze)
	uint8_t c1_rsrc;		///< habilitar a saida do TX em opera��o de loop
	uint8_t c1_m; 			///< 8-bit (0) ou 9-bit de dados
	uint8_t c1_wake;		///< forma de wakeup do RX (0-idle line; 1-address-mark)
	uint8_t c1_ilt;			///< selecionar a forma de deteccao de "idle line"
	uint8_t c1_pe; 			///< habilitar paridade
	uint8_t c1_pt;			///< tipo de paridade (0-par; 1-impar)
	uint8_t c2_rwu; 		///< setar o RX no estado de standby aguardando pelo wakeup
	uint8_t c2_sbk;			///< habilitar o enfileiramento de caracteres break
	uint8_t	s2_msbf;		///< setar endianess para MSB (0 = LSB;1 = MSB)
	uint8_t s2_rxinv;		///< habilitar a inversao da polaridade dos bits do RX
	uint8_t s2_rwuid;		///< habilitar o set do IDLE bit durante standby do RX
	uint8_t s2_brk13;		///< selecionar o comprimento de caracter break (0=10 bits; 1=13 bits)
	uint8_t s2_lbkde;		///< habilitar deteccao de caractere break longo
	uint8_t c3_r8t9; 		///< bit 8 (RX)/bit 9 (TX)
	uint8_t c3_r9t8;		///< bit 9 (RX)/bit 8 (TX)
	uint8_t c3_txdir; 		///< na configuracao RSRC=1, configurar o sentido de TX (0=entrada; 1=saida)
	uint8_t c3_txinv;		///< habilitar a inversao da polaridade dos bits de TX
	uint8_t c4_maen1;		///< habilitar controle de "match address" 1
	uint8_t c4_maen2; 		///< habilitar controle de "match address" 2
	uint8_t c4_m10;			///< selecionar o modo de bits (0=8/9 bits;1=10 bits)
	uint8_t c4_osr;			///< taxa de super-amostragem (default=16(0b01111)
	uint8_t c5_tdmae; 		///< habilitar transmissao por DMA
	uint8_t c5_rdmae; 		///< habilitar recepcao por DMA
	uint8_t c5_bothedge; 	///< amostrar os dados em ambas as bordas do clock de baud rate
	uint8_t c5_resyncdis;	///< desabilitar o resincronismo nas transi��es de 1 para 0
} UART0Config_type;

typedef struct _UARTConfiguration_tag {
	uint8_t	bdh_sbns;		///< selecionar quantidade de stop bits (0 = 1; 1 = 2)
	uint16_t sbr;		
	uint8_t c1_loops;       ///< operacao em loop (normal = 0)
	uint8_t c1_uartswai;	///< comportamento do clock de UART no modo WAIT (continua correndo = 0) 
	uint8_t c1_rsrc;		///< habilitar a saida do TX em opera��o de loop
	uint8_t c1_m; 			///< 8-bit (0) ou 9-bit de dados
	uint8_t c1_wake;		///< forma de wakeup do RX (0-idle line; 1-address-mark)
	uint8_t c1_ilt;			///< selecionar a forma de deteccao de "idle line"
	uint8_t c1_pe; 			///< habilitar paridade
	uint8_t c1_pt;			///< tipo de paridade (0-par; 1-impar)
	uint8_t c2_rwu; 		///< setar o RX no estado de standby aguardando pelo wakeup
	uint8_t c2_sbk;			///< habilitar o enfileiramento de caracteres break
	uint8_t s2_rxinv;		///< habilitar a inversao da polaridade dos bits do RX
	uint8_t s2_rwuid;		///< habilitar o set do IDLE bit durante standby do RX
	uint8_t s2_brk13;		///< selecionar o comprimento de caracter break (0=10 bits; 1=13 bits)
	uint8_t s2_lbkde;		///< habilitar deteccao de caractere break longo
	uint8_t c3_r8; 			///< bit 8 (RX)
	uint8_t c3_t8;			///< bit 8 (TX)
	uint8_t c3_txdir; 		///< na configuracao RSRC=1, configurar o sentido de TX (0=entrada; 1=saida)
	uint8_t c3_txinv;		///< habilitar a inversao da polaridade dos bits de TX
	uint8_t c4_tdmas;		///< selecionar transmissor para transferencia DMA
	uint8_t c4_rdmas; 		///< selecionar receptor para transferencia DMA
} UARTConfig_type;

/*!
 * @brief Configurar o modo de opera��o de UART0
 * @param[in] fonte 
 * @param[in] config parametros do modo de operacao
 */
uint8_t UART0_configure(uint8_t fonte, UART0Config_type *config);

uint8_t UART_configure(uint8_t m, uint8_t fonte, UARTConfig_type *config);

void UART0_initTerminal (UART0Config_type *config);

void UART_initH5Pin2 (UARTConfig_type *config);

void UART0_habilitaIRQTerminal (uint8_t prioridade);

void UART_habilitaIRQH5Pin2 (uint8_t prioridade);

void UART0_habilitaInterruptRxTerminal();

void UART0_desabilitaInterruptRxTerminal();

void UART0_habilitaInterruptTxTerminal();

void UART0_desabilitaInterruptTxTerminal();

#endif /* UART_H_ */
