/*
 * main implementation: use this 'C' sample to create your own application
 *
 */



#include "TPM.h"
#include "SIM.h"
#include "ISR.h"
# include "stdlib.h"
# include "stdint.h"
# include "GPIO_latch_lcd.h"
#include "derivative.h"
#include "util.h"
#include "string.h"

void GPIO_H5Pin3_config_basica () {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK ;    		

	// Funcao GPIO
	PORTE_PCR21 |= PORT_PCR_MUX(0x1); 

	return;
}

void GPIO_H5Pin3_config_especifica() {
	// Sentido do sinal: saida
	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(21));
	
	// Limpar os bits	
	GPIOE_PCOR = GPIO_PCOR_PTCO(GPIO_PIN(21)) ;
}

/**
 * @brief funcao para criar um número randomico cada vez que é chamada (número inteiro entre 0 e 1999)
 */
uint16_t GeraNumeroAleatorio () {
	unsigned int seed;
	uint16_t multiplos;
	seed = (unsigned int) TPMm_leCNT(0);
	srand(seed);
	multiplos = (uint16_t)((rand()%2000));   //max tempo de duracao de espera: 5s. (5/1/400=2000)
	return multiplos;
}

int main(void)
{
	 /* Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);
	
	//Ativa LCD
	GPIO_ativaConLCD ();
	GPIO_initLCD();
	
	/*
	 * Habili																																																																																																																																								|tar sinais de relogio e alocar os pinos
	 * Inicializar os pinos de monitoramento via analisador logico
	 */
	TPM0TPM1_PTAPTE_config_basica ();
//	GPIO_H5Pin3_config_basica ();
	
	/*
	 *  Configurar TPM0 e TPM1
	 */
	TPM_config_especifica (0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.25
	//TPM_config_especifica (1, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.25
	TPM_config_especifica (1, 1489, 0b1111, 0, 0, 0, 0, 0, 0b101);  // (1489*32)/20971520 = 0.002273
	
	TPM_CH_config_especifica (0, 4, 0b0110, 0);				   //OC reseta saida em 0 ao igualar
	/*
	 * Configurar o pino H5Pin2
	 */
//	GPIO_H5Pin3_config_especifica();
	
	/*
	 * Habilitar interrupcoes NVIC
	 */
//	TPM_habilitaNVICIRQ(17, 1);
//	TPM_habilitaNVICIRQ(18, 1);
	TPM_habilitaSwitchesInterrupt (1);
	
	char cedilha[8]={0x00,0x0F,0x10,0x10,0x10,0x1F, 0x06, 0x04};
	char a_tilde[] = {0x02,0x04,0x0E,0x11,0x1F,0x11,0x11,0x00};
	GPIO_escreveBitmapLCD (0x03, (uint8_t *)a_tilde);
	GPIO_escreveBitmapLCD (0x01, (uint8_t *)cedilha);
	char string_01[7] = {'R','e','a',0x01,0x03,'o',0x00};
	
	
	
	/*
	 * Habilitar interrupcoes dos canais
	 */
	uint16_t m;
	uint8_t contador;
	tipo_estado estado = INICIO;
	uint16_t ct1, ct2;
	float intervalo_tempo;
	char intervalo_str[] = "100.00";
	GPIO_escreveStringLCD (0x00, (uint8_t *)"Apertar IRQA12");
	TPM_CH_config_especifica (1, 0, 0b0010, 0);                //IC na borda de descida
	TPM_habilitaInterrupCH(1, 0);
	m = GeraNumeroAleatorio();
	ISR_escreveMultiplos(m);
	
	for(;;) {	
		estado = ISR_LeEstado();
		switch(estado){
		
		case PREPARA_INICIO:
			TPM_CH_config_especifica (1, 0, 0b0010, 0);                //IC na borda de descida
			TPM_habilitaInterrupCH(1, 0);
			GPIO_escreveStringLCD (0x40, (uint8_t *)"                ");
			GPIO_escreveStringLCD (0x00, (uint8_t *)"Apertar IRQA12");
			ISR_EscreveEstado(INICIO);
			m = GeraNumeroAleatorio();
			ISR_escreveMultiplos(m);
			break;
		
		case INICIO:
			break;
			
		case PREPARA_AUDITIVO:
			GPIO_escreveStringLCD (0x00, (uint8_t *)"Teste auditivo");
			ISR_EscreveEstado(ESPERA_ESTIMULO_AUDITIVO);
			//TPM_habilitaInterrupCH(1, 1);
			TPM_habilitaInterrupTOF(1);
			break;
		
		case ESPERA_ESTIMULO_AUDITIVO:
			break;
		
		case RESULTADO:
			ISR_LeContagem (&ct1, &ct2, &contador);
			intervalo_tempo = (float)(((float)contador + (40960.0-(float)ct1+(float)ct2)/40960.0)*0.25);
			ftoa(intervalo_tempo, intervalo_str, 1);				//Converte para string a variavel intervalo_tempo
			//Mostra no display a diferença de tempo entre os acionamentos.
			GPIO_escreveStringLCD (0x40, (uint8_t *)"                   ");
			GPIO_escreveStringLCD(0x40, (uint8_t*) string_01);
			GPIO_escreveStringLCD(0x46, (uint8_t*)" em ");
			GPIO_escreveStringLCD(0x4A,(uint8_t*) intervalo_str);
			GPIO_escreveStringLCD(0x4F, (uint8_t*)"s");
			ISR_EscreveEstado(LEITURA);
			
		
		case LEITURA:
			ISR_resetaTimeOut();
			TPM_CH_config_especifica(0,4,0b0110,0);
			TPM_habilitaInterrupTOF(0);
			ISR_EscreveEstado(ESPERA_LEITURA);
		
		case ESPERA_LEITURA:
			break;
			
		default:
			break;
			
		}

		
	}
	
	return 0;
}
