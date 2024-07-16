
/*!
 * @file GPIO_Switches.c
 * @brief Funcoes relacionadas as botoeiras (incializcao, determinar quais estao pressionadas)
 * @author Fabio e Felipe
 * @date 16/11/2022
 */

/**
 * @brief funcao para inicializacao das botoeiras
 */
#include "derivative.h"
#include "GPIO_Switches.h"
#include "util.h"

void GPIO_initSwitches () {
	// Habilita o clock do modulo PORTA
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;				// para botoeiras   					

	// Funcao GPIO
	PORTA_PCR4 &= ~PORT_PCR_MUX(0x7);			// limpar os bits MUX (0x40049010)
	PORTA_PCR4 |= PORT_PCR_MUX(0x1);			// antes de setar 0b001
	PORTA_PCR5 &= ~PORT_PCR_MUX(0x7);			// limpar os bits MUX (0x40049014)
	PORTA_PCR5 |= PORT_PCR_MUX(0x1);			// antes de setar 0b001
	PORTA_PCR12 &= ~PORT_PCR_MUX(0x7);			// limpar os bits MUX (0x40049030)
	PORTA_PCR12|= PORT_PCR_MUX(0x1);			// antes de setar 0b001

	// Sentido do sinal: entrada (0x400ff014)
	GPIOA_PDDR &= ~(GPIO_PDDR_PDD(GPIO_PIN(4))) &
				~(GPIO_PDDR_PDD(GPIO_PIN(5))) &
				~(GPIO_PDDR_PDD(GPIO_PIN(12)));        
}
/**
 * @brief amostra o registrador GPIOA_PDIR
 * @return valor a partir do qual é possível analisar qual botoeira está pressionada
 */
void GPIO_leSwitchesISF (uint32_t *valor){
	//(*valor =((*((uint32_t volatile*)0x400490A0u)) & 0x1030));
	*valor = PORTA_ISFR & 0x1030;
	
}
/**
 * @brief habilita a interrupcao dawsbotoeiras NMI, IRQA5, IRQA12
 * @param[in] priority nivel de prioridade do atendimento da interrupcao
 */
void GPIO_habilitaSwitchesInterrupt (uint8_t priority) {
	NVIC_ISER |= GPIO_PIN(30);            		// Habilita interrupcao PORTA
	NVIC_ICPR |= GPIO_PIN(30);
	NVIC_IPR7 |= NVIC_IP_PRI_30(priority << 6);

	// Configurar o trigger
	PORTA_PCR4 |= PORT_PCR_ISF_MASK |
			PORT_PCR_IRQC(0b1010);           // "Interrupt on falling edge"

	PORTA_PCR5 |= PORT_PCR_ISF_MASK |
			PORT_PCR_IRQC(0b1010);           // "Interrupt on falling edge"

	PORTA_PCR12 |= PORT_PCR_ISF_MASK |
			PORT_PCR_IRQC(0b1010);           // "Interrupt on falling edge"
}


void GPIO_desativaSwitchesInterrupt (){
	NVIC_ICER |= GPIO_PIN(30);
}

void GPIO_reativaSwitchesInterrupt (){
	NVIC_ISER |= GPIO_PIN(30);
}

void GPIOinitSwitches(){
	// Inicializar
		// Modulo SIM: habilita o clock do modulo PORTA 
		(*(unsigned int volatile *) 0x40048038u) |=  (1<<9);

		// Modulo PORT: configure o pino PTA4,PTA5,PTA12 para GPIO (pino 4 de H5)
		PORTA_PCR4 &= ~0x700; 			// Zera bits 10, 9 e 8 (MUX) de PTA4
		PORTA_PCR4 |= (0b001 << 8);       // Seta bit 8 do MUX de PTA4, assim os 3 bits de MUX serao 001
		PORTA_PCR5  &= ~0x700; 			// Zera bits 10, 9 e 8 (MUX) de PTA5
		PORTA_PCR5  |= (0b001 << 8);       // Seta bit 8 do MUX de PTA5, assim os 3 bits de MUX serao 001
		PORTA_PCR12 &= ~0x700; 			// Zera bits 10, 9 e 8 (MUX) de PTA12
		PORTA_PCR12 |= (0b001 << 8);       // Seta bit 8 do MUX de PTA12, assim os 3 bits de MUX serao 001
		// Modulo GPIO: configure o sentido do sinal no pino PTA 4,5 E 12
		(*(unsigned int volatile *) 0x400FF014u) &= ~0x1030;    // Seta pino 4,5 E 12 do PORTA como entrada
		return;
}
