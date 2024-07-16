/**
 * @file GPIO_latch_lc d.h
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

void GPIO_ativaConLCD ();

void GPIO_setRS (lcd_RS_type i);

/**
 * @brief Inicializa LCD segundo a recomendacao do fabricante
 * @note Veja Secao 10 (pagina 13) da folha tecnica
 * https://www.dca.fee.unicamp.br/cursos/EA871/references/datasheet/AC162A.pdf
 */
void GPIO_initLCD();

void GPIO_escreveByteLatch(uint8_t c);

void GPIO_escreveByteLatchLCD(uint8_t c, uint8_t t);

/**
 * @brief Envia um byte para latch
 * @param[im] c byte
 * @param[[in] t tempo de processamento em multiplos de 10us 
 */
void GPIO_escreveByteLCD(uint8_t c, uint8_t t);

void GPIO_escreveBitmapLCD (uint8_t end, uint8_t* bitmap);

/**
 * @brief Envia uma string a LCD a partir de um dado endereco
 * @param[in] end endereco inicial
 * @param[in] str endereco inicial do vetor de caracteres (string)
 */
void GPIO_escreveStringLCD (uint8_t end, uint8_t* str);

/**
 * @brief Seta endereco de DDRAM no registrador de enderecos do LCD
 * @param[in] end endereco da memoria DDRAM a ser setado
 * @note Consulta os enderecos de cada celula das duas linhas na secao 11 
 * em https://www.dca.fee.unicamp.br/cursos/EA871/references/datasheet/AC162A.pdf
 * 
 */
void GPIO_setEndDDRAMLCD (uint8_t end);

void GPIO_limpaDisplayLCD ();

#endif /* GPIO_LATCH_LCD_H_  */
