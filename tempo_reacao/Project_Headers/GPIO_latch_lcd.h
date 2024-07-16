/*
 * @file 
 * @brief Prototipos, macros e tipos de dados de GPIO_latch_lcd
 * @author Wu Shin Ting
 * @date 28/02/2022
 */

#ifndef GPIO_LATCH_LCD_H_
#define GPIO_LATCH_LCD_H_
#include "stdint.h"

/**
 * @brief Funcao do byte enviado para LCD
 */
typedef enum lcd_RS_tag {
	COMANDO,  //!< Comando (0) 
	DADO	  //!< Dado (1) 
} tipo_lcd_RS;

/*!
 * @brief Habilita conexao entre mcu com latch e LCD.
 */
void GPIO_ativaConLatchLCD ();

/*!
 * @brief Habilita conexao entre mcu e LCD.
 */
void GPIO_ativaConLCD ();

/*!
 * @brief Seta o tipo de informacao enviada para LCD.
 * @param[in] i (COMANDO = 0; DADO = 1)
 */
void GPIO_setRS (tipo_lcd_RS i);

/*!
 * @brief Transfere um byte para LCD.
 * @param[in] c byte
 * @param[in] t tempo de processamento em us
 */
void GPIO_escreveByteLCD(uint8_t c, uint16_t t);

/*!
 * @brief Inicializa LCD
 */
void GPIO_initLCD ();

/*!
 * @brief Escreve uma string de caracteres a partir de um endereco 
 * @param[in] end endereco DDRAM
 * @param[in] str endereco inicial da string
 */
void GPIO_escreveStringLCD (uint8_t end, uint8_t *str);

/*!
 * @brief Envia um byte para latch
 * @param[in] c byte
 */
void GPIO_escreveByteLatch(uint8_t c);

/*!
 * @brief Envia um mesmo byte para latch (ativando LE) e LCD (ativando Enable) 
 * @param[in] c byte
 * @param[in] t tempo de processamento em multiplos de 5us
 */
void GPIO_escreveByteLatchLCD(uint8_t c, uint8_t t);

/*!
 * @brief Transfere o endereco DDRAM
 * @param[in] end endereco da memoria DDRAM
 * 
 * @note Consulta os enderecos de cada celula das duas linhas na 
 * @note secao 11 em ftp://ftp.dca.fee.unicamp.br/pub/docs/ea079/datasheet/AC162A.pdf
 */
void GPIO_setEndDDRAMLCD (uint8_t end);

/*!
 * @brief Constroi um bitmap em CGRAM a partir do endereco end*8 mapeado em end de CGROM
 * @param[in] end CGROM
 * @param[in] bitmap endereco de 8 bytes que definem o bitmap
 */
void GPIO_escreveBitmapLCD (uint8_t end, uint8_t *bitmap);

/******************************************************
 * 
 * Habilita pinos de H2 para analisar os sinais E, RS, LE em relacao a 1 bit de dados
 * 
 ******************************************************/
/**
 * @brief Inicializa pinos de H5
 */
void GPIO_initH5Pins();

/**
 * @brief Seta o tipo de informacao enviada para LCD com o sinal RS espelhado em PTE21.
 * @param[in] i (COMANDO=0, DADO=1)
 */
void GPIO_setRSLCDH5Pin4 (tipo_lcd_RS i);

/**
 * @brief Transfere um byte para latch, LCD e pinos H5 (22=processamento de byte;
 * 21=E, e 20=RS).
 * @param[in] c byte
 * @param[in] t tempo de processamento 
 */
void GPIO_escreveByteLatchLCDH5Pins(uint8_t c, uint16_t t);

/*!
 * @brief escreve uma string de caracteres a partir de um endereco com E e tempo de processamento
 * espelhado em PTE21 e PTE22 
 * @param[in] end endereco DDRAM
 * @param[in] str endereco inicial da string
 */
void GPIO_escreveStringLCDH5Pins (uint8_t end, uint8_t *str);

/**
 * @brief Transfere o endereco DDRAM para LCD com o sinal RS espelhado em PTE20.
 * @param[in] end endereco DDRAM
 */
void GPIO_setEndDDRAMLCDH5Pin4 (uint8_t end);

#endif /* GPIO_LATCH_LCD_H_ */
