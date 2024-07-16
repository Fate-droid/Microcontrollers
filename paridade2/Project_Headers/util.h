/**
 * @file util.h
 * @brief Prototipos, macros e tipos de dados de util.c
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */

#ifndef UTIL_H_
#define UTIL_H_

#define GPIO_PIN(x)  ((1)<<(x))

typedef enum boolean_tag {
	FALSE,     /**< falso/apaga/desativa/liga */
	TRUE      /**< verdadeiro/acende/ativa/fecha */
} tipo_booleano;

typedef enum paridade_tag {
	IMPAR,     /**< falso/apaga/desativa/liga */
	PAR      /**< verdadeiro/acende/ativa/fecha */
} tipo_paridade;

/**
 * @brief espera em multiplos de 5us
 * @param[in] multiplo de 5us
 */
void espera_5us (uint32_t multiplos);

/**
 * @brief espera em decrementos de valores ate 0
 * @param[in] valor de decremento
 */
void espera (uint32_t valor);

/**
 * @brief Converte um valor inteiro para uma string de caracteres ASCII
 * @param[out] s string
 * @param[in] bin valor inteiro
 * @param[in] n quantidade de digitos
 * 
 * @note https://community.arm.com/support-forums/f/keil-forum/17118/conversion-of-integer-to-ascii-for-display
 */
void UlToStr(char *s, uint32_t bin, uint8_t n);

/**
 * @brief Converte segundos em dias
 * @param[in] n segundos
 * @param[out] dias
 * @param[out] horas
 * @param[out] minutos
 * @param[out] segundos
 * 
 * @note https://www.geeksforgeeks.org/converting-seconds-into-days-hours-minutes-and-seconds/
 */
void ConvertSectoDay(uint32_t n, uint32_t *dias, uint32_t *horas, uint32_t *minutos, uint32_t *segundos);


/**
 * @brief Converte segundos em dias no formato HH:MM:SS
 * @param[in] n segundos
 * @return string no formato HH:MM:SS
 * 
 */
char *ConvertSectoDayString (uint32_t seconds, char *string);

/**
 * @brief Converte dias em segundos
 * @param[in] dias
 * @param[in] horas
 * @param[in] minutos
 * @param[in] segundos
 * @param[out] n segundos
 * 
 */
void ConvertDaytoSec(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t *n);

int8_t ConvertBitStringtoU132(char *str, uint32_t *valor);
int8_t ConvertHexStringtoU132(char *str, uint32_t *valor);
int8_t ConvertDecStringtoU132(char *str, uint32_t *valor);
tipo_paridade findParity(uint32_t* x);
char* ConvertU132toBitString(uint32_t n);


#endif /* UTIL_H_ */
