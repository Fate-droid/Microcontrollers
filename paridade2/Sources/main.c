/**
 * @file main.c
 * @brief Este projeto demonstra o uso de um buffer circular para compatibilizar
 * a velocidade de dois dispositivos. Ao digitar uma das seguintes palavras em maiusculas 		
 * VERDE, VINHO, VIOLETA, VERMELHO, VIRIDIANO, são "ecoadas" a palavra digitada 10x no Terminal 
 * @author Wu Shin-Ting
 * @date 05/03/2022
 *
 */
#include "stdlib.h"
#include "string.h"
#include "derivative.h"

#include "UART.h"
#include "util.h"
#include "ISR.h"
#include "buffer_circular.h"


uint8_t i;

int8_t ExtraiString2tokens(char *str, uint8_t *i, char **tokens){
	char delimitador[] = " ";
	char *sub_str[3];
	*i=0;
	sub_str[*i] = strtok(str, delimitador);
	while (sub_str[*i] != '\0' && *i < 2){
		(*i)++;
		sub_str[*i] = strtok(NULL, delimitador);
	}
	if (*i !=2){
		return -1;
	}
	if ((*sub_str[0])!='I' && (*sub_str[0])!='i' && (*sub_str[0])!='P' && (*sub_str[0])!='p'){
		return -3;
		
	}
	tokens[0] = sub_str[0];
	tokens[1] = sub_str[1];
	tokens[2] = '\0';
	return 0;
	
}



/*!
 * @brief Variavel de configuracao de UART0
 */
UART0Config_type config0 = {
		.bdh_sbns=1,		///< Dois stop bit
		.sbr=0b100, 		///< MCGFLLCLK/(baud_rate*osr)=4
		.c1_loops=0,		///< Operacao normal: full-duplex 
		.c1_dozeen=0,		///< UART habilitado no modo WAIT
		.c1_rsrc=0,			///< nao tem efeito para loops == 0
		.c1_m=0,			///< dados em 8 bits
		.c1_wake=0,			///< wakeup de RX por linha ociosa (IDLE)
		.c1_ilt=0,			///< deteccao do estado ocioso pela contagem apos start bits
		.c1_pe=0,			///< sem checagem de paridade por hardware
		.c1_pt=0,			///< bit de paridade par (sem efeito se pe==0)
		.c2_rwu=0,			///< operacao normal de recepcao do UART0
		.c2_sbk=0,			///< operacao normal de transmissor do UART0
		.s2_msbf=0,			///< envio de MSB para LSB
		.s2_rxinv=0,		///< polaridade dos dados de RX nao eh invertida
		.s2_rwuid=0,		///< nao tem efeito para rwu==0
		.s2_brk13=0,		///< nao tem efeito para operacao normal de transmissor
		.s2_lbkde=0,		///< deteccao do caractere break no tamanho menor
		.c3_r8t9=0,			///< nao tem efeito para m==0
		.c3_r9t8=0,			///< nao tem efeito para m==0
		.c3_txdir=0,		///< nao tem efeito para loops==0
		.c3_txinv=0,		///< polaridade dos dados de TX nao eh invertida 
		.c4_maen1=0,		///< todos os dados recebidos sao transferidos para buffer de dados
		.c4_maen2=0,			
		.c4_m10=0,			///< tamanho dos dados em RX e TX é 8 ou 9 bits
		.c4_osr=0b01011,	///< superamostragem 16x
		.c5_tdmae=0,		///< uso de DMA pelo TX desabilitada
		.c5_rdmae=0,		///< uso de DMA pelo RX desabilitado
		.c5_bothedge=0,		///< amostragem somente na borda de subida do clock
		.c5_resyncdis=0		///< resincronizacao na recepcao habilitada
};




// Baud rates: 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
//#define BAUD_RATE 4800
#define BAUD_RATE 38400

/*!
 * @brief Ecoa em linhas de 10 palavras o nome da cor digitado em maíuscula
 */
int main (void)
{	
	/*
	 * Frequencia de configuracao padrao do fabricante: MCGFLLCLK = 20971520Hz
	 */
	
	/*
	 * Configurar UART0 com Rx e Tx habilitados: SBR tem que ser aproximado para maior que baud rate setado
	 */
	config0.sbr = (uint16_t)(20971520./(BAUD_RATE * (config0.c4_osr+1)));
	if ((20971520./(1.0 * BAUD_RATE * (config0.c4_osr+1))) > config0.sbr)
		config0.sbr++;
	UART0_initTerminal (&config0);

	/*!
	 * Habilita IRQs
	 */
	UART0_habilitaIRQTerminal (0);

	// Setup
	ISR_inicializaBC();
	
	/*!
	 * Habilita a interrupcao do Rx do UART0
	 */
	UART0_habilitaInterruptRxTerminal();

	char string_i[16];
	uint8_t i = 0;
	char* tokens[3];
	uint32_t valor = 0;
	char* str_conv[2];
	//uint8_t digitos;
	tipo_paridade a; 
	tipo_paridade b; 
	char string_o[33];
	char str_c[2] = {' ', '\0'};
	char* bin;
	int8_t y,h;
	
	/*!
	 * Reseta o estado do aplicativo
	 */
	ISR_EnviaString ("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
	ISR_escreveEstado (EXPRESSAO);
	string_i[0] = '\0';
	string_o[0] = '\0';

	for(;;) {
		switch(ISR_leEstado()){
		case(MENSAGEM):
			ISR_EnviaString ("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
			ISR_escreveEstado(EXPRESSAO);
			break;
		
		case (TOKENS):
			ISR_extraiString(string_i);
			y =0; //Para zerar mensagens de erros anteriores.
			y = ExtraiString2tokens(string_i, &i, tokens);
			
			if (y==-1){
				ISR_escreveEstado(ERRO);
				ISR_EnviaString("Envie 2 argumentos separados por 'espaço'\n\r");
				break;
			}
			else if(y==-3){
				ISR_escreveEstado(ERRO);
				ISR_EnviaString("Paridade invalida\n\r");
				break;
			}
			else {
				ISR_escreveEstado(COMPUTO);
				break;
			}
		
		case(COMPUTO):		
			str_conv[0] = tokens[1];
			str_c[0] = *(tokens[1]);
			strcpy (string_o, " ");
			
			//Testa se o numero inserido esta na base b, h ou deca
			if ( (str_c[0] == 'b') || (str_c[0] == 'B')){
				h = ConvertBitStringtoU132(str_conv[0],&valor);
			}
			else if ( (str_c[0] == 'h') || (str_c[0] == 'H')){
				h = ConvertHexStringtoU132(str_conv[0],&valor);
			}
			else {
				h = ConvertDecStringtoU132(str_conv[0],&valor);
			}
			
			if(h==-2) {
				ISR_escreveEstado(ERRO);
				ISR_EnviaString("Valor inteiro invalido.\n\r");
				break;
			}
			//Converte a string de 0s e 1s
			bin = ConvertU132toBitString(valor);
			free(bin);
			
			strcat (string_o, bin);
			
			//Computa paridade
			a = findParity(&valor);
			//NUM de bits par (=32)
			b = (a==IMPAR)? PAR:IMPAR;
							
			strcat (string_o," tem paridade ");
			
			if ((*(tokens[0])=='I') || (*(tokens[0])=='i')){ 
				if (a == IMPAR){
					strcat(string_o, "impar em 1");
					}
				else if(b == IMPAR){
					strcat(string_o, "impar em 0");
					}
				}
			else if ((*(tokens[0])=='p') || ((*tokens[0])=='P')){
				if (a==PAR){
					strcat(string_o, "par em 1");
				}
				else if (b==PAR){
					strcat(string_o, "par em 0");
				}
			}
			
			strcat (string_o, "\n\r");
			ISR_escreveEstado(RESULTADO);
			ISR_EnviaString(string_o);
			
			break;
		
			
		case (RESULTADO):
			break;
		
		case (EXPRESSAO):
			break;
		default:
			break;
	
		}
	}	
	return 0;
}
