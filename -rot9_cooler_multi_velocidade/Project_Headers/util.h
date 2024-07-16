/*!
 * @file util.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo util.c
 * @author Wu Shin Ting
 * @date 27/02/2022
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "stdint.h"

/*!
 * @def GPIO_PIN
 * @brief Constroi uma mascara com somente bit x em 1  
 */
#define GPIO_PIN(x)  ((1)<<(x)) 

/*!
 * @enum boolean_tag
 * @brief Estados booleanos
 */
typedef enum boolean_tag {
	OFF,     ///< falso/apaga/desativa/liga/low 
	ON,      ///< verdadeiro/acende/ativa/fecha/high
} booleano_type;

/*!
 * @brief insere um tempo de espera multiplos*10us no fluxo de controle
 * @param[in] multiplos de 10us 
 */
void delay_2us (uint32_t multiplos);

/*!
 * @brief Converte segundos para formato (DIA:)HH:MM:SS
 * @param[in] seconds segundos
 * @param[out] string horario no formato HH:MM:SS
 */
char *ttoa (uint32_t seconds, char *string);

/*!
 * @brief convert a unsigned integer number into a string
 * @param[in] n number
 * @param[in] b system base
 * @param[out] string converted number
 */
char *itoa (int32_t n, char *string, char b);

/*!
 * @brief Converte segundos para unidades de tempo, dias, horas, minutos e segundos
 * @param[in] seconds horario eem segundos
 * @param[out] DD dias
 * @param[out] HH horas
 * @param[out] MM minutos
 * @param[out] SS segundos
 */
void s2dhms (uint32_t segundos, uint32_t *DD, uint32_t *HH, uint32_t *MM, uint32_t *SS);


/*!
 * @brief Converte hor�rio em unidades de tempo, dias, horas, minutos e segundos, para segundos
 * @param[in] DD dias
 * @param[in] HH horas
 * @param[in] MM minutos
 * @param[in] SS segundos
 * @param[out] seconds horario eem segundos
 */
void dhms2s (uint8_t DD, uint8_t HH, uint8_t MM, uint8_t SS, uint32_t *segundos);

/**
 * @brief ...
 * @param[in]
 * @param[out]
 * @param[in]
 */
int intToStr(int x, char str[], int d);

/**
 * @brief ...
 * @param[in]
 * @param[out]
 * @param[in]
 */
void ftoa(float n, char* res, int afterpoint);

//void ftoa(float n, char* res, int afterpoint);
//int intToStr(int x, char str[], int d);
//void reverse(char* str, int len);


#endif /* UTIL_H_ */
