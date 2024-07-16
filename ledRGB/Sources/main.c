/* @file main.c
 * @brief Leds verde e vermelho piscante
 * @author Wu Shin-Ting
 * @date 06/01/2023
 */

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief Conffiguracao especifica dos modulos 
 */
void config_especifica() {
	/**
	 * Configura o modulo GPIOB: sentido de saida do sinal no pino PTB18
	 * registrador GPIOB_PDDR mapeado em 0x400FF054u
	 */
	(*(uint32_t volatile *) 0x400FF054u) |= (1<<18);
	
	/**
	 * Configura o modulo GPIOB: sentido de saida do sinal no pino PTB19
	 */
	(*(uint32_t volatile *) 0x400FF054u) |= (1<<19);
	
	
	/**
	 * Configura o modulo GPIOD: sentido de saida do sinal no pino PTD1
	 * registrador GPIOB_PDDR mapeado em 0x400FF0D4u
	*/
	(*(uint32_t volatile *) 0x400FF0D4u) |= (1<<1);
		
	
	/**
	 * Configura o modulo GPIOA: sentido de entrada do sinal no pino PTA4, sem
	 * interrupcao habilitada
	 * registrador GPIOA_PDDR mapeado em 0x400ff014u
	 */
	(*(uint32_t volatile *) 0x400ff014u) &= ~(1<<4);
			
	/**
	 * Configura o modulo GPIOA: sentido de entrada do sinal no pino PTA5, com interrupcao
	 * habilitada
	 * registrador GPIOA_PDDR mapeado em 0x400ff014u
	 */
	(*(uint32_t volatile *) 0x400ff014u) &= ~(1<<5);	
	
	(*(uint32_t volatile *) 0x400ff014u) &= ~(1<<12);

	/**
	 * Habilita a interrupcao do pino para borda de descida
	 */
	/*pino5*/
	*(uint32_t volatile *) 0x40049014u &= ~0xF0000; 
	(*(uint32_t volatile *) 0x40049014u) |= 0x000B0000; 
	
	/*pino4*/
	*(uint32_t volatile *) 0x40049010u  &= ~0xF0000; 
	(*(uint32_t volatile *) 0x40049010u) |= 0x000B0000; 
	
	/*pino12*/
	*(uint32_t volatile *) 0x40049030u &= ~0xF0000; 
	(*(uint32_t volatile *) 0x40049030u) |= 0x000B0000; 

	/**
	 * Configura o modulo NVIC: habilita IRQ 30
	 * registrador NVIC_ISER mapeado em 0xe000e100u
	 */
	(*(uint32_t volatile *) 0xe000e100u) |= (1<<30);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ 30
	 * registrador NVIC_ICPR mapeado em 0xe000e280u
	 */
	(*(uint32_t volatile *) 0xe000e280u) |= (1<<30);	

	/**
	 * Configura o modulo NVIC: seta prioriodade 3 para IRQ30
	 * registrador NVIC_IPR7 mapeado em 0xe000e41cu (Tabela 3-7/p. 54 do Manual)
	 */
	(*(uint32_t volatile *) 0xe000e41cu) |= (0b11<<22); 	//(Secao 3.3.2.1/p. 52 do Manual)
}

/**
 * @brief Configuracao basica de operacao dos modulos
 */
void config_basica () {
	/**
	 * Configura o modulo SIM: ativacao dos sinais de relogio de PORTB e PORTD
	 */
	*(uint32_t volatile *) 0x40048038u |= (1<<10);
	*(uint32_t volatile *) 0x40048038u |= (1<<12);
	/**
	 * Ativacao dos sinais de relogio de PORTA
	 */
	*(uint32_t volatile *) 0x40048038u |= (1<<9);

	/**
	 * Configura o modulo PORTB_PCR18
	 */
	*(uint32_t volatile *) 0x4004A048u  &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTB18
	(*(uint32_t volatile *) 0x4004A048u) |= 0x00000100; // Seta bit 8 do MUX de PTB18
	
	/**
	 * Configura o modulo PORTD_PCR1
	 */
	*(uint32_t volatile *) 0x4004C004u  &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTD1
	(*(uint32_t volatile *) 0x4004C004u) |= 0x00000100; // Seta bit 8 do MUX de PTD1
	
	/**
	 * Configura o modulo PORTB_PCR19
	 */
	*(uint32_t volatile *) 0x4004A04Cu &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTB19
	(*(uint32_t volatile *) 0x4004A04Cu) |= 0x00000100; // Seta bit 8 do MUX de PTB19
	
	/**
	 * Configura o modulo PORTA_PCR4
	 * registrador PORTA_PCR4 mapeado em 0x40049010 
	 */
	*(uint32_t volatile *) 0x40049010u &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTA4
	(*(uint32_t volatile *) 0x40049010u) |= 0x00000100; // Seta bit 8 do MUX de PTA4

	/**
	 * Configura o modulo PORTA_PCR5
	 * registrador PORTA_PCR5 mapeado em 0x40049014 
	 */
	*(uint32_t volatile *) 0x40049014u &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTA5
	(*(uint32_t volatile *) 0x40049014u) |= 0x00000100; // Seta bit 8 do MUX de PTA5
	
	/**
	 * Configura o modulo PORTA_PCR12
	 */
	*(uint32_t volatile *) 0x40049030u &= ~0x700; // Zera bits 10, 9 e 8 (MUX) de PTA12
	(*(uint32_t volatile *) 0x40049030u) |= 0x00000100; // Seta bit 8 do MUX de PTA12
}

/**
 * @inicializacao do estado do sistema
 */
void inicializacao () {
	/**
	 * Inicializa o estado PTB18 em 1 (botoeira ativo baixo)
	 */
	(*(uint32_t volatile *) 0x400FF044u) |= (1<<18);

	/**
	 * Inicializa o estado PTB19 em 1 (botoeira ativo baixo)
	 */
	(*(uint32_t volatile *) 0x400FF044u) |= (1<<19);
	
	/**
	 * Inicializa o estado PTD1 em 1 (botoeira ativo baixo)
	 */
	(*(uint32_t volatile *) 0x400FF0C0u) |= (1<<1);
}

/**
 * @definicao da rotina de servico declarada
 */
void PORTA_IRQHandler (){
	/**
	 * Busca o bit de estado correspondente
	 */
	if ((*(uint32_t volatile *) 0x40049014u) & 0x1000000) {
		
		if((*(uint32_t volatile *) 0x400FF010u)&0x20){
			(*(uint32_t volatile *)0x400FF04Cu) |= (1 << 19);			
		}
		/**
		 * Limpa flag 
		 */
		(*(uint32_t volatile *) 0x40049014u) |= (1<<24);
	}
	
	if ((*(uint32_t volatile *) 0x40049010u) & 0x1000000) {
			
		if((*(uint32_t volatile *) 0x400FF010u)&0x10){
			(*(uint32_t volatile *)0x400FF04Cu) |= (1 << 18);			
		}
		/**
		 * Limpa flag 
		 */
		(*(uint32_t volatile *) 0x40049010u) |= (1<<24);
	}

	if ((*(uint32_t volatile *) 0x40049030u) & 0x1000000) {
				
		if((*(uint32_t volatile *) 0x400FF010u)&0x1000){
			(*(uint32_t volatile *)0x400FF0CCu) |= (1 << 1);			
		}
		/**
		 * Limpa flag 
		 */
		(*(uint32_t volatile *) 0x40049030u) |= (1<<24);
	}

}
/**
 * @brief Fluxo de controle principal
 */
int main(void)
{
	config_basica();

	config_especifica();

	inicializacao();

	for(;;){

	}
	return(0);

}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
