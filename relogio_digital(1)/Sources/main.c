/*
 * @file main.c
 * @brief Este projeto relogio_digital demonstra uma aplicacao dos modulos PIT e RTC.
 * @date 
 * @author 
 *
 */

#include "SIM.h" 
#include "PIT.h"
#include "RTC.h"
#include "ISR.h"
#include "GPIO_latch_lcd.h"
#include "GPIO_switches.h"
#include "util.h"

void atualizaHorarioLCD (uint32_t segundos) {
	char hh_mm_ss[9];
	
	GPIO_escreveStringLCD (0x00, (uint8_t *)ConvertSectoDayString(segundos, hh_mm_ss));
}

int main(void)
{
	//Bloco 1: Inicializar (Configurar os modulos necessarios)
	//Define a frequencia do sinal do barramento
	SIM_setaOUTDIV4 (0b000);    //bus clock = 20971520/4 = 5242880

	//Inicializa conexao com LCD
	GPIO_ativaConLCD ();

	//Inicializa LCD
	GPIO_initLCD ();

	//Inicializa chave IRQA5 com borda de descida como evento de interrupcao (0b1010)
	//e prioridade=3
	GPIO_initSwitches(0b1010, 2);

	//Inicializa timer 0 do PIT
	PIT_initTimer0(57671680, 1);  //periodo = 2.75* 20971520 
	//com o timer0 desabilitado
	PIT_desativaTimer0 ();
	//Habilita pinos 3 e 4 para analise da configuracao do periodo e periodo*postscaler

	//Inicializa o modulo RTC com fonte LPO
	RTClpo_init();
	//RTC_ativaSegundoIRQ (3);
	//Habilita pinos 1 e 2 para analise da configuracao do periodo

	//Bloco 2: Fluxo de controle da tarefa de relogio
	uint32_t anterior, atual;
	uint32_t dias, horas_anterior_local, minutos_anterior_local, segundos_anterior_local, horas_atual_local, minutos_atual_local, segundos_atual_local;
	char str_unid_hor[3];
	tipo_estado estado;

	//Inicializacoes
	RTClpo_setTime (12345);
	anterior = RTClpo_getTime (&atual);
	ISR_escreveEstado (NORMAL);
	str_unid_hor[2] = '\0';

	//Inicializa o vetor de unidades de tempo do dia com os segundos de RTC 
	ISR_carregaHorario();
	ISR_leHorario(&dias, &horas_atual_local, &minutos_atual_local, &segundos_atual_local);

	for(;;) {	   
		horas_anterior_local = horas_atual_local; 
		minutos_anterior_local = minutos_atual_local; 
		segundos_anterior_local = segundos_atual_local;
		
		//GPIO_desativaSwitches ();
		GPIO_desativaSwitchesNVICInterrupt();
		estado = ISR_LeEstado ();
		ISR_leHorario (&dias, &horas_atual_local, &minutos_atual_local, &segundos_atual_local);
		GPIO_reativaSwitchesNVICInterrupt();
		//GPIO_reativaSwitches(0b1010);
		
		switch (estado) {
		case NORMAL:
			RTClpo_getTime (&atual);
			if (atual > anterior ) {
				
				atualizaHorarioLCD (atual);

				//Atualiza o tempo anterior
				anterior = atual;
			}
			break;
		case INCREMENTE_HORA:
			if (horas_atual_local != horas_anterior_local) {
				str_unid_hor[0] = (horas_atual_local < 10)? '0': (horas_atual_local/10)+'0';
				str_unid_hor[1] = horas_atual_local%10+'0';
				GPIO_escreveStringLCD (0x00, (uint8_t *)str_unid_hor);
				ISR_escreveEstado (INCREMENTE_HORA_ESPERA);
	
				//Atualiza o tempo anterior
				ConvertDaytoSec(dias, horas_atual_local, minutos_atual_local, segundos_atual_local, &anterior);
			}
			
			
			break;
		case INCREMENTE_MINUTO:
			if (minutos_atual_local != minutos_anterior_local) {
				str_unid_hor[0] = (minutos_atual_local < 10)? '0': (minutos_atual_local/10)+'0';
				str_unid_hor[1] = minutos_atual_local%10+'0';
				GPIO_escreveStringLCD (0x03, (uint8_t *)str_unid_hor);
				ISR_escreveEstado (INCREMENTE_MINUTO_ESPERA);

				//Atualiza o tempo anterior
				ConvertDaytoSec(dias, horas_atual_local, minutos_atual_local, segundos_atual_local, &anterior);
			}
			break;
		case INCREMENTE_SEGUNDO:
			if (segundos_atual_local != segundos_anterior_local) {
				str_unid_hor[0] = (segundos_atual_local < 10)? '0': (segundos_atual_local/10)+'0';
				str_unid_hor[1] = segundos_atual_local%10+'0';
				GPIO_escreveStringLCD (0x06, (uint8_t *)str_unid_hor);
				ISR_escreveEstado (INCREMENTE_SEGUNDO_ESPERA);

				//Atualiza o tempo anterior
				ConvertDaytoSec(dias, horas_atual_local, minutos_atual_local, segundos_atual_local, &anterior);
			
			
			break;
			}
		default:
			break;	
		}
	}

	return 0;
}
