/*!
 * @file ISR.c
 * @brief Este modulo contem os programas de tratamento de instrução usados; como também as funções de leitura e escrita de estados do jogo.
 * @author Fabio e Felipe
 * @date 16/11/2022
 */

# include "derivative.h"
#include "ISR.h"
#include "stdint.h"
#include "GPIO_Switches.h"
#include "TPM.h"
#include "main.h"
#include "MKL25Z4.h"

static estado_type estado;
static uint8_t multiplos;
static uint8_t contador;
static uint8_t botoeira;
static uint16_t first_value;
static uint16_t second_value;
static uint16_t diff_counter;
static TPM_MemMapPtr TPM[] = TPM_BASE_PTRS;
static uint16_t timeout;

void ISR_EscreveEstado(estado_type valor){
	estado = valor;
}

estado_type ISR_LeEstado (){
	return estado;
}

void ISR_EscreveMultiplos(uint8_t valor){
	multiplos = valor;
}

void ISR_LeContagem (uint8_t *botoeira_arg, uint16_t *primeiro, uint16_t *segundo, uint16_t *ciclos){
	*botoeira_arg = botoeira;
	*primeiro = first_value;
	*segundo = second_value;
	*ciclos = diff_counter; 
}

void ISR_ResetTimeout (){
	timeout = 0;
}
void FTM0_IRQHandler (){
	
	if(TPM0_STATUS & TPM_STATUS_CH1F_MASK){
		if (estado == LEDSWAIT){
			first_value = TPM0_C1V;
			diff_counter = 0;
			botoeira = 0;
			TPM_desativaCh(0,1);
			
			TPM[0]->SC |= (TPM_SC_TOIE_MASK);			 // habilita interrupcao por overflow.
			
			estado = LEDSGO;
			
		} else if (estado == LEDSGO) {
			second_value = TPM0_C1V;
			TPM_desativaCh(0,1);
			
			TPM[0]->SC &= ~(TPM_SC_TOIE_MASK); 			// Desabilita interrupcao por overflow.
			estado = CONTAGEM;
		}
		TPM0_C1SC |= TPM_CnSC_CHF_MASK;					//Limpa pendencias do canal 1 modulo 0.

	} 
	else if (TPM0_STATUS & TPM_STATUS_CH2F_MASK) {
		if(estado == LEDSWAIT){
			first_value = TPM0_C2V;
			diff_counter = 0;
			botoeira = 1;
			TPM_desativaCh(0,2);
			
			TPM[0]->SC |= (TPM_SC_TOIE_MASK); 			// Habilita interrupcao por overflow.
			estado = LEDSGO;
		}
		else if(estado == LEDSGO){
			second_value = TPM0_C2V;
			TPM_desativaCh(0,2);
			
			TPM[0]->SC &= ~(TPM_SC_TOIE_MASK); 			// Desabilita interrupcao por overflow.
			estado = CONTAGEM;
		}
		TPM0_C2SC |= TPM_CnSC_CHF_MASK;				// Limpa pendencias no canal 2 modulo 0.
		
	}
	else if(TPM0_STATUS & TPM_STATUS_TOF_MASK){
		diff_counter++;
		TPM[0]->SC |= (TPM_SC_TOF_MASK);			 // Limpa pendencias do overflow aplicando w1c no bit 7
	}
}

void FTM1_IRQHandler (){
	uint16_t valor;

	if(TPM1_STATUS & TPM_STATUS_CH0F_MASK){
		
		valor = TPM_LeCNT(2);
		TPM_ativaChOC(2,0,0b0110, valor);		//Ativa canal de LED R em OC (junto com interrupcao)
		TPM_desativaCh(1,0);					//Desativa o canal da botoeira IRQA12;

		contador = 0;
		estado = RANDOM;
		TPM1_C0SC |= TPM_CnSC_CHF_MASK;			//Limpa pendencias no canal 0 modulo TPM1.	
		
		
	}
}

void FTM2_IRQHandler(){

	if(TPM2_STATUS & TPM_STATUS_CH0F_MASK) {
		contador++;
		
		if (contador >= multiplos) {
			TPM_desativaCh(2,0);
			TPM_ativaChOC(2,1, 0b0110, TPM2_C0V);
			
			
			estado = LEDSWAIT;
	        contador = 0;
		}
		TPM2_C0SC |= TPM_CnSC_CHF_MASK;
	}
	else if (TPM2_STATUS & TPM_STATUS_CH1F_MASK){
		if (estado == LEDSWAIT){
			contador++;
			if (contador ==1){
				TPM_ativaChIC (0,2, 0b0010);
				TPM_ativaChIC (0,1, 0b0010);
			}
		}
			else if (estado == CONTAGEMWAIT){
				timeout++;
				if (timeout > 15){
					TPM_desativaCh(2,1);
					estado = RESULTADO;
				}
			}
		
		TPM2_C1SC |= TPM_CnSC_CHF_MASK;
	}
}

	


