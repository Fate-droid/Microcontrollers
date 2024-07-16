/*!
 * @file util.c
 * @brief Este modulo contem as funcoes de uso frequente
 * @author Wu Shin Ting
 * @date 17/09/2022
 */

#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "util.h"

void delay_2us (uint32_t i) {

	__asm__ (								
			"mov  r3, #0\n\t"					      
			"iteracao:\n\t"

			"mov	r2, #5\n\t"			  
			"laco:\n\t" 
			"add		r3, #0\n\t"     
			"sub		r3, #0\n\t"
			"add		r3, #0\n\t"     
			"sub		r3, #0\n\t"
			"add		r3, #0\n\t"     
			"sub 		r2, #1\n\t"					  

			"bne 	laco\n\t"					  

			"sub	%0, #1\n\t"					  	  
			"bne	iteracao\n\t"

			:
			: "r" (i)			 //i recebe o codigo %0
			  : "r2","r3" //os registradores usados no escopo __asm__
	);
}

char *itoa (int32_t n, char *string, char b)
{
	char sinal;
	unsigned short i, j, a;
	unsigned int m;
	
	if (n == 0) {
		///< Tratamento do caso especial
		string[0] = '0';
		string[1] = '\0';
		return string;
	}
	
	if (n < 0) sinal = 1;
	else sinal = 0;
	
	/*!
	 * Converter para codigos ASCII, digito por digito, de menos 
	 * significativo para o mais
	 */ 
	i = 0;
	m = abs(n);
	
	while (m != 0) {
		a = m%b;
		string[i+sinal] = (a>9)?(a-10)+'A' : a+'0';
		i++;
		m = m/b;
	}
	string[i+sinal] = '\0';			///< Incluir terminador
	
	///< Reverter a sequencia
	j = strlen(string)-1;
	i = sinal;
			
	while (j > i) {
		char tmp = string[i];
		string[i] = string[j];
		string[j] = tmp;
		i++;
		j--;
	}
	
	///< Adicionar o sinal negativo caso necessario
	if (sinal) string[0] = '-';
	
	return string;
}

char *ttoa (uint32_t seconds, char *string)
{
	uint32_t dd, hh, mm, ss;

	s2dhms (seconds, &dd, &hh, &mm, &ss);
	
	string[2] = string[5] = ':';    // inserir os espacadores
	string[8] = '\0';				// terminador

	if (hh > 23) {
		//!< horario invalido: FF:FF:FF
		string[0] = string[1] = string[3] = string[4] = string[6] = string[7] = 'F';
	}
	
	string[0] = (hh < 10)? '0': (hh/10)+'0';
	string[1] = hh%10+'0';
	
	string[3] = (mm < 10)? '0': (mm/10)+'0';
	string[4] = mm%10+'0';
	
	string[6] = (ss < 10)? '0': (ss/10)+'0';
	string[7] = ss%10+'0';	
	
	return string;
}

void s2dhms (uint32_t segundos, uint32_t *DD, uint32_t *HH, uint32_t *MM, uint32_t *SS)
{
	*DD = segundos/86400;

	uint32_t sec = segundos%86400;
	
	*SS = sec%60;
	
	*MM = (sec/60)%60;
	
	*HH = sec/3600;
	
}

void dhms2s (uint8_t DD, uint8_t HH, uint8_t MM, uint8_t SS, uint32_t *segundos)
{
	*segundos = DD*86400+HH*3600+MM*60+SS;
}

//char *ftoa (float v, char *string)
//{
//	unsigned short i, j, a;
//	unsigned int m;
//	uint8_t x = (v*10);
//	m = abs(x);
//	if ((10*v-x)>= 0.5){
//		m++;
//	}
//	string[0] = '0';
//	string[1] = '\0';
//	m = m/10;
//	i = 0;	
//	while (m != 0) {
//		a = m%10;
//		string[i] = (a>9)?(a-10)+'A' : a+'0';
//		i++;
//		m = m/10;
//	}
//	string[i] = '\0';			///< Incluir terminador
//
//	///< Reverter a sequencia
//		j = strlen(string)-1;
//		i=0;
//				
//		while (j > i) {
//			char tmp = string[i];
//			string[i] = string[j];
//			string[j] = tmp;
//			i++;
//			j--;
//		}
//		
//		return string;
//	}



int intToStr(int x, char str[], int d)
{   int j, temp;
    int i = 0;
    int k = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
 
    while (i < d)
        str[i++] = '0';
 
    ///< Reverter a sequencia
    j = i-1;
    while (k < j) {
        temp = str[k];
        str[k] = str[j];
        str[j] = temp;
        k++;
        j--;
    }
    
    str[i] = '\0';
    return i;
}
 
void ftoa(float n, char* res, int afterpoint)
{
    int ipart = (int)n;
 
    float fpart = n - (float)ipart;
    
    //Ting: A ultima casa de uma valor em ponto flutuante eh arredondada conforme
    //o que eh truncado ...
    fpart = fpart * pow(10, afterpoint);
    uint32_t fpart_int = (uint32_t)fpart;
    if (fpart - fpart_int >= 0.5) fpart_int++;
 
    int i = intToStr(ipart, res, 0);
 
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        //Ting: Converter o valor arredondado para ficar similar ao computo que a maquina
        //faria ...
        //fpart = fpart * pow(10, afterpoint); 
        //intToStr((int)fpart, res + i + 1, afterpoint);
        intToStr(fpart_int, res + i + 1, afterpoint);
    }
}
