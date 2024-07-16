/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date Jan 11, 2023
 * @author Wu Shin-Ting   
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"


void espera_5us (uint32_t multiplos)
{
		__asm__ (
			// substitua pelo seu codigo do roteiro 3
			"NOP \n\t"
		);
}

void espera (uint32_t valor)
{
	while (valor) valor--;
}

int8_t ConvertBitStringtoU132(char *str, uint32_t *valor){
	int8_t digitos = strlen(str);
	uint8_t i;
	uint32_t pot = 1;
	*valor =0;
	if (digitos > 32 ) return -2;
	
	for(i=0;i<digitos-1; i++){
		uint8_t aux = digitos-i-1;
		if (str[aux]!= '0' && str[aux] != '1')	return -2;
		*valor += (str[aux]-0x30)*pot;
		pot*= 2;
	}
	return 0;
}
int8_t ConvertHexStringtoU132(char *str, uint32_t *valor){
	int8_t digitos = strlen(str);
	uint8_t i;
	uint32_t pot=1;
	*valor =0;
	if (digitos > 8){
				return -2;
			}
	for (i=0; i<(digitos-1); i++){
		uint8_t aux = digitos-i-1;

		if (!((str[aux] > 0x29 && str[aux] < 0x3A) ||
				(str[aux] > 0x40 && str[aux] < 0x47) ||
				(str[aux] > 0x60 && str[aux] < 0x67))) return -2;

		if (str[aux]<0x3a){
			*valor += (str[aux]-0x30)*pot;
		}
		else if (str[aux]<0x47){
			*valor += (str[aux]-0x37)*pot;
			//Considerando minusculas:
		} else if (str[aux]<0x67) {
			*valor += (str[aux]-0x57)*pot;
		}

		pot*=16; 
	}

	return 0;
}
int8_t ConvertDecStringtoU132(char *str, uint32_t *valor){
	int8_t digitos = strlen(str);
	uint8_t i;
	uint32_t pot =1;
	*valor =0;
	if (digitos > 9){
		return -2;
	}
	for (i=0; i<digitos; i++){
		uint8_t aux = digitos-i-1;
		if(str[aux] < 0x30 || str[aux] > 0x39) return -2;

		*valor += (str[aux]-0x30)*pot;
		pot*=10; 
	}
	
	return 0;
}

 
// Calcula a paridade de um número `x` usando a tabela de pesquisa. Devolve TRUE quando há um número ímpar de 1s.
tipo_paridade findParity(uint32_t *x)
{
    // divide recursivamentemente o inteiro (32 bits) em dois
    // reduz pela metade e pega seu XOR até restar apenas 1 bit
 
    *x = (*x & 0x0000FFFF) ^ (*x >> 16);
    *x = (*x & 0x000000FF) ^ (*x >> 8);
    *x = (*x & 0x0000000F) ^ (*x >> 4);
    *x = (*x & 0x00000003) ^ (*x >> 2);
    *x = (*x & 0x00000001) ^ (*x >> 1);
 
    // retorna 1 se o último bit estiver definido; caso contrário, retorne 0
    if ((*x & 1)==1) return IMPAR;
    return PAR;
}
 
//char* toBinaryString(uint32_t n) {
//  char *string = malloc(sizeof(uint32_t)*8 + 1);
//  uint8_t k;
//  if (!string) {
//    return NULL;
//  }
//  for (k = 31; k >= 0;k--) {
//    string[k] = (n & 1)+0x30;
//    n >>= 1;
//  }
//  
//  return string;
//}

char* ConvertU132toBitString(uint32_t n) {
  uint8_t num_bits = sizeof(int) * 8;
  int32_t i;
  char *string = malloc(num_bits + 1);
  if (!string) {
    return NULL;
  }
  for (i = 31; i >= 0; i--) {
    string[i] = (n & 1) + '0';
    n >>= 1;
  }
  string[num_bits] = '\0';
  return string;
}

void UlToStr(char *s, uint32_t bin, uint8_t n)
{
    s += n;
    *s = '\0';

    while (n--)
    {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
    
}

void ConvertSectoDay(uint32_t n, uint32_t *days, uint32_t *hours, uint32_t *minutes, uint32_t *seconds)
{
    *days = n / (24 * 3600);
 
    n = n % (24 * 3600);
    *hours = n / 3600;
 
    n %= 3600;
    *minutes = n / 60 ;
 
    n %= 60;
    *seconds = n;     
}

char *ConvertSectoDayString (uint32_t seconds, char *string)
{
	uint32_t dd, hh, mm, ss;

	ConvertSectoDay (seconds, &dd, &hh, &mm, &ss);

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

void ConvertDaytoSec(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t *n)
{
	*n = days*86400+hours*3600+minutes*60+seconds;
}
