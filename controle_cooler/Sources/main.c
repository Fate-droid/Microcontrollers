/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "GPIO_latch_lcd.h"
#include "TPM.h"
#include "TPM.h"
#include "ISR.h"
#include "ADC.h"
#include "SIM.h"
#include "util.h"


 //Configuracao do ADC0
struct ADC_MemMap Master_Adc_Config = {
		.SC1[0]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.SC1[1]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.CFG1=ADLPC_NORMAL
		 | ADC_CFG1_ADIV(ADIV_8)
		 | ADLSMP_SHORT
		 | ADC_CFG1_MODE(MODE_16)
		 | ADC_CFG1_ADICLK(ADICLK_BUS_2),   
		.CFG2=MUXSEL_ADCA					//select ADC0_SE9 (PTB1 potenciometro externo)
		 | ADACKEN_DISABLED
		 | 0                                //| ADHSC_HISPEED
		 | ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x1234u,                                   
		.CV2=0x5678u,
		.SC2=ADTRG_HW //Hardware trigger
		 | ACFE_DISABLED
		 | ACFGT_LESS
		 | ACREN_DISABLED
		 | DMAEN_DISABLED
		 | ADC_SC2_REFSEL(REFSEL_EXT),                                    
		.SC3=CAL_OFF
		 | ADCO_SINGLE
		 | AVGE_ENABLED
		 | ADC_SC3_AVGS(AVGS_16),
};

void tensao2duty_cycle(float valor, float *dc){
	//Ting: Pensem um pouco se voces obteriam a mesma fracao com ADC0_RA/0xFFFF???)
	*dc = (float) (valor/(0xFFFF))*1;
}


int main(void)
{
	GPIO_ativaConLCD();
	GPIO_initLCD();
	/*
	 * Configura fonte de pulsos para contadores de TPMs (MCGFLLCLK)
	 */
	SIM_setaTPMSRC (0b01);

	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
    TPM1TPM2_PTB0PTB18PTB19_config_basica();
	
	TPM_config_especifica (1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b110);
	TPM_config_especifica (2, 65535, 0b1111, 0, 0, 0, 0, 0, 0b110);
	TPM_CH_config_especifica (1, 0, 0b1010, 0);    
	//TPM_CH_config_especifica (2, 0, 0b0110, 0); 
	//TPM_CH_config_especifica (2, 1, 0b0110, 0); 
	TPM_CH_config_especifica (2, 0, 0b1011, 0); 
    TPM_CH_config_especifica (2, 1, 0b1011, 0); 

	
	// Ativa sinais de relogio e multiplexa pino para canal 9 (SE9) com disparo HW (TPM2)
	ADC_PTB1_config_basica(TPM2_TRG);
	
	// Configuracao especifica e calibracao do modulo ADC
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);		// configura via "drive ADC"
	ADC_Cal (ADC0_BASE_PTR);								// calibra (Secao 28.4.6/p.494 no Manual)
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);  	 //reconfigura
	ADC_selecionaCanal (0b01001); 
	
	//Habilita interrupcoes no NVIC
	ADC_habilitaNVICIRQ(2);		
	
	// Habilita interrupcao do ADC
	ADC_habilitaInterrupCOCO();

	
	
	//Declaracao de variaveis uteis
	tipo_estado estado;
	float temperatura_atual;
	uint16_t tensao_amostra;
	uint16_t temperatura_Volt;
	float duty_cycle;
	char temp_str[]="25.60 C";
	char duty_str[]="25.00";
	uint16_t* valores;
	
	
	ISR_EscreveEstado(AMOSTRA_VOLT);
	
	for(;;) {	   
		estado = ISR_LeEstado();
		if (estado == ATUALIZACAO) {
			//Carregamento de entradas
			valores = ISR_leValoresAmostrados();
			tensao_amostra = valores[0];
			temperatura_Volt = valores[1];
			
			//Procesamento das entradas para ajuste de perifericos
			temperatura_atual = AN3031_temperatura(temperatura_Volt);
			atualiza_velocidade(tensao_amostra);
			atualiza_leds(temperatura_atual);
			
			//Procesamento das entradas para renderizacao no LCD
			ftoa(temperatura_atual,temp_str,2);
			tensao2duty_cycle(tensao_amostra, &duty_cycle);
			ftoa(duty_cycle,duty_str,2);
			
			//Renderiza no LCD
			GPIO_escreveStringLCD (0x05,(uint8_t *) temp_str);
			GPIO_escreveStringLCD (0x0A,(uint8_t *) " C");
			GPIO_escreveStringLCD (0x45,(uint8_t *) duty_str);
			GPIO_escreveStringLCD (0x4A,(uint8_t *) " ");
		
		//tarefas de atualização do LCD e dos sinais de saída para
		//cooler e leds.
			ISR_EscreveEstado(AMOSTRA_VOLT);
		}
	}
	
	return 0;
}
