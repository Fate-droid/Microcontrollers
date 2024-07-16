/*!
 * @file main.c
 * @brief Este modulo contem o programa do fluxo princial como também funções de geração de números aleatórios.
 * @author Fabio e Felipe
 * @date 16/11/2022
 */

# include "ISR.h"
# include "TPM.h"
#include "derivative.h" /* include peripheral declarations */
# include "stdlib.h"
# include "stdint.h"
# include "GPIO_latch_lcd.h"

/**
 * @brief funcao para criar um número randomico cada vez que é chamada (número inteiro entre 4 e 13)
 */
uint8_t StartRandom () {
	unsigned int seed;
	uint8_t multiplos;
	seed = (unsigned int) TPM_LeCNT(2);
	srand(seed);
	multiplos = (uint8_t)((rand()%10)+4);
	return multiplos;
}


/**
 * @brief programa principal do projeto jogo de destreza.
 */
int main(){
	//Ativa LCD
	GPIO_ativaConLCD ();
	GPIO_initLCD();
	
	
	//Inicializa canais associadas às switches e leds R e V.
	TPM_initSwitchesIC (65535, 0b111);
	TPM_initPTB18PTB19OC (65535, 0b111);
	
	//Habilita interrupção do modulo NVIC de maneira a tratar interrupções de Switches e leds.
	TPM_habilitaSwitchesInterrupt (1);
	TPM_habilitTPM2CH0TPM2CH1Interrupt (3);
	
	//Ativa botoeira IRQA12 modo IC
	TPM_ativaChIC (1,0, 0b0010);
	
	//estado inicial
	ISR_EscreveEstado(INICIAL);
	
	//Definição de variaveis locais usadas no loop for.
	estado_type estado;
	uint16_t primeiro, segundo, diff;
	float intervalo_tempo;
	uint8_t bot;
	char intervalo_str[]="100.00";
	uint8_t multiplos;
	
	multiplos = StartRandom(); 						//Inicialza uma variavel que armazena o num. aleatorio 
	ISR_EscreveMultiplos(multiplos);
	
	for (;;) {
		estado = ISR_LeEstado();
		switch (estado) {
		case INICIAL:
			TPM_ativaChIC (1,0, 0b0010);			//Ativa IRQA12
			GPIO_escreveStringLCD (0x00, (uint8_t *)"Pressione IRQA12");
			
			break;
			
		case RANDOM:
			
			break;
		case LEDSWAIT:
			GPIO_escreveStringLCD (0x00, (uint8_t *)"                ");
			
			break;
		case LEDSGO:
			
			break;
		case CONTAGEMWAIT:
			
			//Mostra no display qual botoeira foi apertada primeiro.
			if (bot == 0){
				GPIO_escreveStringLCD (0x00, (uint8_t *)"NMI Primeiro!");
			}
			else if (bot ==1) {
				GPIO_escreveStringLCD (0x00, (uint8_t *)"IRQA5 Primeiro!");
				
			}
			
			ftoa(intervalo_tempo, intervalo_str, 2);				//Converte para string a variavel intervalo_tempo
			//Mostra no display a diferença de tempo entre os acionamentos.
			GPIO_escreveStringLCD (0x40, (uint8_t *)"Dif: ");
			GPIO_escreveStringLCD (0x45, (uint8_t *) intervalo_str);
			GPIO_escreveStringLCD (0x4B, (uint8_t *)" ms");
			
			
			break;
			
		case RESULTADO:
			//Prepara todas as variáveis e exibição no display antes de ir para estado inicial.
			multiplos = StartRandom();
			ISR_EscreveMultiplos(multiplos);
			GPIO_escreveStringLCD (0x40, (uint8_t *)"                ");
			
			ISR_EscreveEstado(INICIAL);
			ISR_ResetTimeout ();				//Reseta o tempo de timeout do led verde.
			
			break;
			
		case CONTAGEM:
			
			//Computa o valor em ms da diferença de tempo entre acionamentos.
			ISR_LeContagem (&bot, &primeiro, &segundo, &diff);
			if (primeiro > segundo) {
				intervalo_tempo = (((float)(65535+segundo-primeiro)/65535 + (diff-1))* 0.3999)*1000.0;
			}
			else if (segundo >= primeiro){
				intervalo_tempo = (((float)(segundo-primeiro)/65535 + diff)* 0.3999)*1000.0;
			}
			ISR_EscreveEstado(CONTAGEMWAIT);
			
			break;
	   }
	}
}
