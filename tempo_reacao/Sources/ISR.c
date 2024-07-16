/*
 * ISR.c
 *
 *  Created on: Feb 27, 2023
 *      Author: ea871
 */
# include "derivative.h"
#include "MKL25Z4.h"
#include "TPM.h"
#include "stdint.h"
#include "ISR.h"


static tipo_estado estado;
static uint8_t tempo_reacao, time_out; 
static uint16_t tempo_aleatorio;
static TPM_MemMapPtr TPM[] = TPM_BASE_PTRS;
static uint16_t ct1, ct2;

void ISR_EscreveEstado(tipo_estado valor){
	estado = valor;
}

tipo_estado ISR_LeEstado (){
	return estado;
}

void ISR_LeContagem (uint16_t *primeiro, uint16_t *segundo, uint8_t *counter){
	*primeiro = ct1;
	*segundo = ct2;
	*counter = tempo_reacao;
}

void ISR_resetaTimeOut(){
	time_out=0;
	
}

void ISR_escreveMultiplos(uint16_t valor){
	tempo_aleatorio = valor;
	
}

void FTM0_IRQHandler (){
	
	if(TPM0_STATUS & TPM_STATUS_TOF_MASK){
		if (estado == ESPERA_REACAO_AUDITIVA) tempo_reacao++;
		else if(estado == ESPERA_LEITURA){
			time_out++;	
			if (time_out ==12){
				TPM_desativaCh(0,4);
				TPM_desabilitaInterrupTOF(0);
				estado = PREPARA_INICIO;
			}
		}
		TPM[0]->SC |= (TPM_SC_TOF_MASK);			//Limpa pendencias 
	} 
	
	else if (TPM0_STATUS & TPM_STATUS_CH1F_MASK) {
		if (estado == ESPERA_REACAO_AUDITIVA){
		ct1 = TPM0_C4V;
		ct2 = TPM0_C1V;
		TPM_desativaCh(1,1); //Desativa canal do buffer
		TPM_desativaCh(0,1);//Desativa canal do NMI
		TPM_desativaCh(0,4);
		TPM_desabilitaInterrupTOF(0);
		TPM[0]->CONTROLS[1].CnSC |= (TPM_CnSC_CHF_MASK);	// resetar flag
		estado = RESULTADO;
		}
	}
}

//ISR do modulo com um canal em IC
void FTM1_IRQHandler (){

	if(TPM1_STATUS & TPM_STATUS_CH0F_MASK){
		
		TPM_desativaCh (1, 0);
		TPM_CH_config_especifica (1, 1, 0b1010, 0);			//Sem interrupcao habilitada
		estado = PREPARA_AUDITIVO;
		TPM[1]->CONTROLS[0].CnSC |= (TPM_CnSC_CHF_MASK);	// resetar flag
	}
	
	
	if(TPM1_STATUS & TPM_STATUS_TOF_MASK) {
		if(estado == ESPERA_ESTIMULO_AUDITIVO){
		tempo_aleatorio--;
		
		if (tempo_aleatorio == 0) {
			TPM[1]->CONTROLS[1].CnV = TPM_CnV_VAL(1000);
			TPM_CH_config_especifica (0, 1, 0b0010, 0);				//Habilita botoeira NMI en IC.
			TPM_CH_config_especifica (0, 4, 0b0110, TPM[0]->CNT);	
			TPM_habilitaInterrupCH(0, 1);
			TPM_habilitaInterrupCH(0, 4);
			TPM_habilitaInterrupTOF(0);
			TPM_desabilitaInterrupTOF(1);
			tempo_reacao = 0;
			estado = ESPERA_REACAO_AUDITIVA;
		}
		TPM[1]->SC |= (TPM_SC_TOF_MASK);	
		}
		
	}
	
	
}

