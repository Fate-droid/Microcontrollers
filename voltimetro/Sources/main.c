/*!

 * @file main.c
 * @brief Projeto Voltimetro - EA871
 * @author Felipe e Fabio
 * @date 03/11/2022
 *
 */

#include "ADC.h"
#include "timers.h"
#include "GPIO_latch_lcd.h"
#include "ISR.h"
#include "util.h"
#include "GPIO_ledRGB.h"
#include "TPM.h"


ADCConfig config = {
		.sc1_diff=0,      		///< conversao unipolar (0) ou diferencial (1)
		.cfg1_adlpc=0,			///< potencia normal
		.cfg1_adiv=0b10,		///< 4
		.cfg1_adlsmp=1, 		///< tempo de amostragem longo (!!!)
		.cfg1_mode=0b11,		///< 8 bits (unipolar)/9 bits (bipolar)   (!!!)
		.cfg1_adiclk=0b01,		///< bus clock/2
		.cfg2_muxsel=0, 		///< canal A (0)
		.cfg2_adacken=0,		///< desabilitar a saida de sinal de clock assincrono
		.cfg2_adhsc=0,			///< desabilitar operacao em alta velocidade
//		.cfg2_adhsc=1,			///< habilitar operacao em alta velocidade
		.cfg2_adlsts=0b11, 		///< tempo de amostragem mais longo
		.sc2_adtrg=0,			///< trigger por software
		.sc2_acfe=0, 		    ///< desabilitar funcao de comparacao em igualdade
		.sc2_acfgt=0,			///< desabilitar funcao de comparacao em desigualdade
		.sc2_acren=0,			///< desabilitar comparacao de uma faixa
		.sc2_dmaen=0, 			///< habilitar de DMA
		.sc2_refsel=0b00,		///< tensoes de referencia VREFH e VREFL
		.sc3_adco=0,			///< amostragem unica
		.sc3_avge=1, 			///< habilitar media de amostras por hardware
		.sc3_avgs=0b00			///< 8 amostras (!!!)
//		.sc3_avgs=0b11 			///< 32 amostras
};

float V_REFH = 3.28;		//Tensao de referencia em V
//#define VREFMAX =(0XFFFF);

/**
 * @brief Conversao de codigo amostrado para tensao
 * @param[in] valor binario amostrado pelo ADCO
 * @param[in] conteudo da variavel tipo float que armazena o resultado
 */

void codigo2tensao(uint16_t valor, float *mV){
	*mV = (float) (((float) valor)/((float)(0xFFFF)))*V_REFH;
}

float vtemp_25 = 0.703125;


/**
 * @brief Conversao de codigo amostrado pelo canal para temperatura
 * @param[in] valor binario amostrado pelo ADCO (canal conectado ao sensor de temp. )
 * @param[in] conteudo da variavel tipo float temperatura que armazena o resultado
 */

void AN3031_temperatura (uint16_t valor, float *temperatura) {
	float m;
	float valor_analog;
	codigo2tensao(valor, &valor_analog);
	if (valor_analog >= vtemp_25){
		m = 1.646;
	}
	else {
		m = 1.769;	
	}
	
	*temperatura = 25.-(valor_analog - vtemp_25)/m;
}


/**
 * @brief Conversao linear do valor de tensao detectado no PTB0, para valor de Duty cycle
 * @param[in] valor analogico de tensao
 * @param[in] conteudo da variavel tipo float dc que armazena o resultado
 */
void tensao2duty_cycle(float valor, float *dc){
	*dc = (float) (valor/(V_REFH))*1;
}


/**
 * @brief Fluxo principal do programa
 * @return 1 somente para satisfazer a sintaxe C
 */
int main(void)
{
	SIM_setaOUTDIV4 (0b001); 			//divide clock de barramento por 2.
	
	GPIO_ativaConLatchLCD ();
	GPIO_initLCD();
	GPIO_initledRGB();
	
	PIT_initSemAtivacaoTimer0(898001); 
	ADC0_init(&config);	
	
	ADC0_connectPTB1ToChannel ();
	ADC0_initADHWT(0b100);  
	
	//PWM
	uint32_t ps = 64;
	TPM_initPTB0EPWM (65534, ps, 0, 1);
	
	//Set-up
	uint16_t amostras[2] = {0, 0};
	estado_type estado;
	float duty_corrente;

	ADC0_selChannel (0b01001); 				//!< /li /l selecionar o canal do header H7
	ADC0_ativaIRQ(2);
	PIT_ativaIRQ(1);
	
	//Ativa PIT
	PIT_ativaTimer0();
	
	GPIO_ativaConLCD ();
	
	//Declaracoes de variaveis uteis
	float amostra_V;
	float temperatura;
	char str_T[]="65536";
	char str_TEMP[]="TEMP:";
	char str_C[]="C";
	char str_DUTY[]="DUTY";
	char str_duty[]="99.99";
			
	//Reseta o estado
	amostras[0] = amostras[1] = 0;
		
	for(;;) {
		ISR_LeValoresAmostrados (amostras);
		estado = ISR_leEstado();
		
		if (estado == ATUALIZACAO) {
			//converte os valores amostrados
			codigo2tensao(amostras[0], &amostra_V);
			AN3031_temperatura (amostras[1], &temperatura);
			
			//mostra Temp no visor LCD
			ftoa(temperatura,str_T,2);
			GPIO_escreveStringLCD (0x02, (uint8_t *) str_TEMP);
			GPIO_escreveStringLCD (0x07, (uint8_t *) str_T);
			GPIO_escreveStringLCD (0x0C, (uint8_t *) str_C);
			
			//indicacao de limite de temperatura
			if (temperatura > 25) {
				GPIO_ledRGB(1,0,0);
			}
			
			//Atualiza duty cycle
			tensao2duty_cycle(amostra_V, &duty_corrente);
			TPM_chaveiaChModoNivel (1, 0, 0b1010);
			TPM_atualizaDutyCycleEPWM(1, 0, duty_corrente);
			
			//mostra Duty cycle no visor LCD
			ftoa(duty_corrente,str_duty,2);
			GPIO_escreveStringLCD (0x43,(uint8_t *) str_DUTY);
			GPIO_escreveStringLCD (0x47,(uint8_t *) str_duty);
			estado = ESPERA;
		}
	}
	return 0;
}
	




