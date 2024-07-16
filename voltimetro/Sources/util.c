/*!
 * @file util.c
 * @brief Este modulo contem as funcoes de uso frequente
 * @author Fabio e Felipe
 * @date 03/11/2022
 */

#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "util.h"

void delay_10us (unsigned int multiplos)
{
	
	__asm__ (								
			"mov  r3, #0\n\t"					      
			"iteracao:\n\t"
			
			"mov	r2, #26\n\t"			  
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
			: "r" (multiplos)			 //i recebe o codigo %0
			: "r2","r3" //os registradores usados no escopo __asm__
	);
}

char *itoa (int n, char *string, char b)
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
 
    int i = intToStr(ipart, res, 0);
 
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
