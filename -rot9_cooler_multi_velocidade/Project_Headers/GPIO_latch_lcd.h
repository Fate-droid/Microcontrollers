/**
 * @file GPIO_latch_lcd.h
 * @brief Programas relacionados com o processamento do LCD
 * @author Wu Shin Ting
 * @date 12/08/2022
 */

#ifndef GPIO_LATCH_LCD_H_
#define GPIO_LATCH_LCD_H_

//Para uso dos tipos de dados uint*_t
#include "stdint.h"

/**
 * @brief Funcao do byte enviado para LCD
 */
typedef enum lcd_RS_tag {
	COMANDO,  //!< Comando (0) 
	DADO	  //!< Dado (1) 
} lcd_RS_type;

/*!
 * @brief Ativa conexao entre mcu com latch e lcd 
 */
void GPIO_ativaConLatchLCD ();
/**
 * @brief  Inicializacao pinos LCD: Configura os pinos conectados em Latch e LCD com funcao GPIO e determina o sentido do sinal nos pinos PORTC0-10 como saida
 */
void GPIO_ativaConLCD ();
/**
 * @brief Determina se o LCD deve ser setado no modo de comando ou de dados
 */
void GPIO_setRS (lcd_RS_type i);

/**
 * @brief Inicializa LCD segundo a recomendacao do fabricante
 * @note Veja Secao 10 (pagina 13) da folha tecnica
 * https://www.dca.fee.unicamp.br/cursos/EA871/references/datasheet/AC162A.pdf
 */
void GPIO_initLCD();
/**
 * @brief Escreve o byste fornecido (c) no LCD
 * @param[im] c byte
 * @param[[in] t tempo de processamento em multiplos de 2us 
 */
void GPIO_escreveByteLatchLCD(uint8_t c, uint8_t t);

/**
 * @brief Envia um byte para latch
 * @param[im] c byte
 * @param[[in] t tempo de processamento em multiplos de 2us 
 */
//Ting: Veja a explicacao em GPIO_limpaDisplayLCD ...
//void GPIO_escreveByteLCD(uint8_t c, uint8_t t);

void GPIO_escreveByteLCD(uint8_t c, uint16_t t);

/**
 * @brief Grava o bitmap no LCD
 * @param[in] end endereco da memoria CGRAM a ser setado
 * @param[in] vetor bitmap
 */
//void GPIO_leByteLCD(uint8_t c, uint8_t t);

void GPIO_escreveBitmapLCD (uint8_t end, uint8_t* bitmap);

/**
 * @brief Envia uma string a LCD a partir de um dado endereco
 * @param[in] end endereco inicial
 * @param[in] str endereco inicial do vetor de caracteres (string)
 */
void GPIO_escreveStringLCD (uint8_t end, uint8_t* str);
//void GPIO_leStringLCD (uint8_t end, uint8_t* str);

/**
 * @brief Seta endereco de DDRAM no registrador de enderecos do LCD
 * @param[in] end endereco da memoria DDRAM a ser setado
 * @note Consulta os enderecos de cada celula das duas linhas na secao 11 
 * em https://www.dca.fee.unicamp.br/cursos/EA871/references/datasheet/AC162A.pdf
 * 
 */
void GPIO_setEndDDRAMLCD (uint8_t end);
/**
 * @brief limpa o display LCD
 */
void GPIO_limpaDisplayLCD ();

//Ting: passei para main.h
//void constroiBitmaps (char *ends, char *arcof, char *f_acima, char *f_abaixo);

#endif /* GPIO_LATCH_LCD_H_  */
