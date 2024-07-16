/*
 * @file buffer_circular.c
 * @brief Este arquivo contem as funcoes de processamento de buffer circular.
 * @author Wu Shin-Ting
 * @date 21/06/2021
 * 
 */

#include "stdlib.h"
#include "buffer_circular.h"
#include "stdint.h"

void BC_init(BufferCirc_type *buffer, unsigned int tamanho) 
{
	buffer->dados = malloc(tamanho * sizeof(char));
	buffer->tamanho = tamanho;
	buffer->escrita = 0;
	buffer->leitura = 0;
}

void BC_reset(BufferCirc_type *buffer) 
{
	buffer->escrita = buffer->leitura = 0;
}

void BC_free(BufferCirc_type *buffer)
{
	buffer->tamanho = 0;
	buffer->escrita = 0;
	buffer->leitura = 0;
	//Liberar o espaco de memoria ocupado pelos dados
	free(buffer->dados);
}

int BC_push (BufferCirc_type *buffer, char item)
{
	unsigned int next;

	next = buffer->escrita+1;
	if (next == buffer->tamanho) next = 0;

	if (next == buffer->leitura) {
		//sem espaco no buffer
		return -1;
	}
	buffer->dados[buffer->escrita] = item;
	buffer->escrita = next;

	return 0;
}

int BC_pop (BufferCirc_type *buffer, char *item)
{
	unsigned int next;

	if (buffer->escrita == buffer->leitura) {
		//buffer vazio
		buffer->escrita = buffer->leitura = 0; //reset
		return -1;
	}
	next = buffer->leitura + 1;
	if (next == buffer->tamanho) next = 0;
	*item = buffer->dados[buffer->leitura];
	buffer->leitura = next;	

	return 0;
}

unsigned int BC_elements (BufferCirc_type *buffer)
{ 
	unsigned int tmp;
	if (buffer->escrita > buffer->leitura) tmp = buffer->escrita - buffer->leitura;
	else tmp = (buffer->tamanho - buffer->leitura) + buffer->escrita;

	return tmp;
}

uint8_t BC_isFull (BufferCirc_type *buffer)
{
	unsigned int next = buffer->escrita+1;
	if (next == buffer->tamanho) next = 0;

	if (next == buffer->leitura) {
		//sem espaco no buffer
		return 1;
	}

	return 0;
}

uint8_t BC_isEmpty (BufferCirc_type *buffer)
{
	if (buffer->leitura == buffer->escrita) {
		return 1;
	}
	return 0;
}



