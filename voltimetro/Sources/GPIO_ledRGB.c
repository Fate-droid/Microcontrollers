/**
 * @file GPIO_ledRGB
 * @brief Programas relacionados à inicialização e uso do led RGB da placa.
 * @Todas as funções implementadas foram similares às do roteiro 6.
 * @author Fabio Bareiro Fleitas
 * @date 05/10/2022
 */

#include "stdint.h"
#include "GPIO_ledRGB.h"

/**
 * @brief
 * @Inicializa os led RGB da placa.
 */
int GPIO_initledRGB(void)
{

    /* Write your code here */
	// Bloco 1: Inicializar
	// Modulo SIM: habilita o clock dos modulos PORTE, PORTB e PORTD.
	(*(unsigned int volatile *) 0x40048038u) |=  (1<<13);
	(*(unsigned int volatile *) 0x40048038u) |=  (1<<10);
	(*(unsigned int volatile *) 0x40048038u) |=  (1<<12);

	// Modulo PORT: configure o pino PTE20 para GPIO (pino 4 de H5. Configure tambem os pinos 18, 19 e 1 com instruções similares (mascaramento e uso do operador shift)
	(*(unsigned int volatile *) 0x4004D050u) &= ~0x700; 			// Zera bits 10, 9 e 8 (MUX) de PTE23
	(*(unsigned int volatile *) 0x4004D050u) |= (0b001 << 8);       // Seta bit 8 do MUX de PTE23, assim os 3 bits de MUX serao 001
	(*(unsigned int volatile *) 0x4004A048u) &= ~0x700; 			// 
	(*(unsigned int volatile *) 0x4004A048u) |= (0b001 << 8); 
	(*(unsigned int volatile *) 0x4004A04Cu) &= ~0x700; 			//
	(*(unsigned int volatile *) 0x4004A04Cu) |= (0b001 << 8); 
	(*(unsigned int volatile *) 0x4004C004u) &= ~0x700; 			//
	(*(unsigned int volatile *) 0x4004C004u) |= (0b001 << 8); 
	
	
	// Modulo GPIO: configure o sentido do sinal para os pinos do LEDRGB (PORTB18, PTB19 e PT1). 
	(*(unsigned int volatile *) 0x400FF114u) |= (1<<20);    
	(*(unsigned int volatile *) 0x400FF0D4u) |= (1<<1);   
	(*(unsigned int volatile *) 0x400FF054) |= (0b11)<<18;
	
	return 0;
}

/**
 * @brief
 * @Função que ativa as cores do led de acordo com os bits passados pelos seus argumentos.
 */
void GPIO_ledRGB(uint8_t estadoR,uint8_t estadoG,uint8_t estadoB)
{

(*(unsigned int volatile *) 0x400FF044u) = (0b11<<18);

(*(unsigned int volatile *) 0x400FF0C4u) = (1<<1);

	if (estadoR ==1){ 
			// Escrever 0 no pino PTB18
			(*(unsigned int volatile*) 0x400FF048u) = (1<<18); 		
	}
	
	if (estadoG == 1) {
			// Escrever 0 no pino PTB19
			(*(unsigned int volatile*) 0x400FF048) = (1<<19);
	}
	
	if (estadoB ==1) {
			// Escrever 0 no pino PTD1
			(*(unsigned int volatile*) 0x400FF0C8) = (1<<1);
	}
}

/**
 * @brief
 * @Converte o codigo de um estado (ou cor) para um conjunto xyz de bits que indicam à função GPIO_ledRGB
 * quais leds acender. Chama a funçãp GPIO_ledRGB no final.
 */
void ativa_cor(uint8_t codigo_cor)
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
	
	x = (codigo_cor & 0b100)>> 2;
	y = (codigo_cor & 0b010)>> 1;
	z = (codigo_cor & 0b001)>> 0;

	GPIO_ledRGB(x,y,z);
}
