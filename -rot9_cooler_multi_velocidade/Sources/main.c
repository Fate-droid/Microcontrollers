
#include "timers.h"
#include "TPM.h"
//#include "derivative.h"
#include "GPIO_Switches.h"
#include "GPIO_latch_lcd.h"
#include "ISR.h"
#include "util.h"

//Ting: Basta incluir o arquivo que contem a definicao dos tipos de dadso uint*_t
#include "stdint.h"

#include "main.h"

// questao 4
//int main (void)
//{
//	uint16_t periodo;
//	uint8_t ps, percentagem, polaridade;
//	for (;;) {
//		TPM_initPTB0EPWM (periodo, ps, percentagem, polaridade);
//	}
//}

// questao 5

//int main (void)
//{
//	uint32_t ps = 64;
//	uint8_t percentagem=50;
//	TPM_initPTB0EPWM (65535, ps, 0, 1);
//	for (;;) {
//		TPM_atualizaDutyCycleEPWM (1, 0, percentagem);
//	}
//}

//questao 6
//int main (void)
//{
//	char string[7];
//	float valor;
//	for (;;) {
//		ftoa (valor, string, 2);
//	}
//	return 0;
//}

//questao 7
//int main (void)
//{
//	uint8_t MbaEba;
//	uint32_t ps = 64;
//	TPM_initPTB0EPWM (65534, ps, 0, 1);
//	for (;;) {
//		TPM_chaveiaChModoNivel (1, 0, MbaEba);
//	}
//}

//questao 8

//int main (void){
//	GPIO_ativaConLatchLCD();
//	GPIO_ativaConLCD();
//	GPIO_initLCD();
//	char arcof[8] = {0x08, 0x07, 0x03, 0x06, 0x0A, 0x12, 0x04, 0x08};
//	char f_acima[8] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x00, 0x00};
//	char f_abaixo[8] = {0x00, 0x00, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04};
//	uint8_t  ends[4]={0x02, 0x03, 0x04, 0x78}; 								//ultimo endereço eh a da cgrom
//	constroiBitmaps ((char *)ends, arcof, f_acima, f_abaixo);
//	
//	
//	GPIO_escreveStringLCD(0x40, (uint8_t*) ends);
//	for (;;) {
//		
//		}
//	
//	return 0;
//}

//questao 11


void constroiBitmaps (char *ends, char *arcof, char *f_acima, char *f_abaixo){
		GPIO_escreveBitmapLCD (ends[0], (uint8_t *) arcof);
		GPIO_escreveBitmapLCD (ends[1], (uint8_t *) f_acima);
		GPIO_escreveBitmapLCD (ends[2], (uint8_t *) f_abaixo);
		
		//Ting: Se quiserem construir um quarto bitmap ... basta incluir mais uma instrucao ...
		//GPIO_escreveBitmapLCD (ends[3], (uint8_t *) f_quarto);		
	}

int main(){
	//Ting: Valor absoluto seria a contagem que esta setado no contador TPM1_Ch0
	//float absoluta, razao;
	uint16_t absoluta;
	float razao;
	
	//Ting: O contador tem ate 16 bits. Portanto os valores sao < 65536!!! NO maximo 5 digitos
	//char str1[]="100.1000";
	char str1[]="65536";
	//Ting: A razao nao deveria ser entre 0 e 1????
	//char str2[]="1.10";
	char str2[]="1.00";
	   
	//Ting: Para ver o nome simbolico por que nao usar o tipo estado_type? Fica mais facil depurar ...
	//Como duty varia de 0 a 100 ... basta um tipo uint8_t ... Certo?
	//uint32_t estado_corrente, duty_corrente, estado_anterior, duty_anterior;
	estado_type estado_corrente, estado_anterior;
	uint8_t duty_corrente, duty_anterior;
	
	//Ting: Nao deveriam ser  bitmaps? Nao deveriam passar 4 vetores de bitmaps???
	char arcof[8] = {0x08, 0x07, 0x03, 0x06, 0x0A, 0x12, 0x04, 0x08};
	char f_acima[8] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x00, 0x00};
	char f_abaixo[8] = {0x00, 0x00, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04};
	uint8_t  ends[4]={0x02, 0x03, 0x04, 0x78}; 								//ultimo endereço eh a da cgrom
	constroiBitmaps ((char *)ends, arcof, f_acima, f_abaixo);
	
	//PWM
	uint32_t ps = 64;
	TPM_initPTB0EPWM (65534, ps, 0, 1);
	
	//Inicializacoes
	GPIOinitSwitches();
	GPIO_habilitaSwitchesInterrupt(3);
	GPIO_ativaConLCD();
	GPIO_initLCD();
	
	//Le estado e duty corrente(valor inicial) e renderiza-os no LCD
	estado_corrente = ISR_leEstado();
	duty_corrente = ISR_leDuty ();
	estado_anterior = estado_corrente;
	duty_anterior = duty_corrente;
	GPIO_escreveStringLCD (0x0f, &ends[3]);
	
	absoluta = duty_corrente*0.01*TPM_leMOD(1);
	razao = absoluta/(TPM_leMOD(1) + 1);
	ftoa(absoluta, str1, 2);
	GPIO_escreveStringLCD (0x40, (uint8_t *)str1);
	ftoa(razao, str2, 2);
	GPIO_escreveStringLCD (0x4C, (uint8_t *)str2);
	
	//Ting: nao eh mais necessario
	//uint8_t h;
	for (;;) {
		GPIO_desativaSwitchesInterrupt ();	
		estado_corrente = ISR_leEstado();
		duty_corrente = ISR_leDuty ();
		GPIO_reativaSwitchesInterrupt ();
		
		if (estado_corrente != estado_anterior) {
			//Ting: Veja em ISR.h que troquei a ordem dos nomes simbolicos ... que me permitiu
			//usar diretamente a variavel estado_corrente ... Veja https://www.geeksforgeeks.org/enumeration-enum-c/
			//para maiores detalhes de enum 
//			switch (estado_corrente){ 
//				case DESLIGADO:
//					h=3;
//					break;
//				case RESET:
//					h=0;
//					break;
//				case DECREMENTA:
//					h=2;
//					break;
//				case INCREMENTA:
//					h=1;
//					break;
//			}
			
			//Ting: &ends[h] equivale a pegar o endereco do elemento <h> do vetor ends 
			//e lembrando que escever uma string sempre comeca a partir do endereco inicial
			//do vetor e vai ate o terminador '\0' (veja a definicao de GPIO_escreveStringLCD) ...
			//Veja na sua definicao de ends, o ultimo elemento do vetor eh '\0' (=0x00)?
//			GPIO_escreveStringLCD (0x0f, &ends[h]);
			
			//Ting: O que vcs querem escrever? Nao eh somente um byte? Nao basta passar o endereco do byte?
			//Por que nao usar a funcao que escreve um byte?
			//Ting: Setar o cursor na celula correta
			GPIO_setEndDDRAMLCD (0x0f);   
			//Ting: Chaveiar para modo DADO
			GPIO_setRS (DADO);
			//Ting: 
			GPIO_escreveByteLCD(ends[(uint8_t)estado_corrente], 22);   //Ting: 43us/2 -> 22
			
			estado_anterior = estado_corrente;
		}
	

		if (duty_corrente != duty_anterior) {
			if (estado_corrente == RESET) {
				//chaveia canal 0 para o modo EPWM;
				TPM_chaveiaChModoNivel (1, 0, 0b1010);
				TPM_atualizaDutyCycleEPWM(1, 0, duty_corrente);
			
			}
			if (estado_corrente == DESLIGADO) { 
				TPM_chaveiaChModoNivel(1, 0, 0b0000);
			}		
			
			//Ting: Veja no manual que limpar LCD eh "custoso" em tempo 
			//Ting: Ao inves de limpar o LCD escreve 1 espacos brancos ...
			//Limpa o LCD e escreve novamente a string indicadora de estado.
			//GPIO_limpaDisplayLCD ();
			GPIO_escreveStringLCD(0x40, (uint8_t *)"                ");
			
			//Ting: Veja que o tempo de espera de processamento ja esta integrado em GPIO_limpaDisplayyLCD!
			//delay_2us(10000);				//Delay para evitar falta de tempo para execução da funcao GPIO_limpaLCD

			//Ting: O bitmap ja nao foi atualizado no bloco de if acima?
			//GPIO_escreveStringLCD (0x0f, &ends[h]);
			
			//absoluta = TPM_leChValor (1, 0);
			//Ting: Faltou a ultima unidade ...
//			absoluta = 0.01*duty_corrente*TPM_leMOD(1);
			absoluta = 0.01*duty_corrente*(TPM_leMOD(1)+1);
					
			//Ting: O resultado da divisao de dois valores inteiros eh inteiro 
			//Para que o resultado seja um fracionario eh necessario "forcar" que um valor
			//tenha a parte fracionaria!!!
			//razao = absoluta/(TPM_leMOD(1) + 1);
			razao = ((float)absoluta)/(TPM_leMOD(1) + 1);
			
			//Reenderiza absoluta e razao no LCD.
			//Ting: o valor absoluto eh a contagem (inteira) do contador (TPM1_Ch0) ...
			//ftoa(absoluta, str1, 2);
			itoa (absoluta, str1, 10);
			GPIO_escreveStringLCD (0x40, (uint8_t *)str1);
			
			ftoa(razao, str2, 2);
			GPIO_escreveStringLCD (0x4C, (uint8_t *)str2);
			duty_anterior = duty_corrente;
		}
	}
}

//Teste do ftoa
//int main()
//{
//    char res[6];
//    float valor = 233.007;
//    for (;;){
//    	ftoa(valor, res, 2);
//    }
//    	
//    return 0;
//}
