/*!
 * @file ISR.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo ISR.c
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag {
	MENSAGEM,
	EXPRESSAO,
	TOKENS,
	COMPUTO,
	RESULTADO,
	ERRO
} tipo_estado;

//#define POLLING_MODE 1
//#define INTERRUPT_MODE 2
//
//#define UART_MODE POLLING_MODE //INTERRUPT_MODE
////#define UART_MODE INTERRUPT_MODE


/*!
 * @brief Le o estado do aplicativo
 */
tipo_estado ISR_leEstado ();

/*!
 * @brief Atualiza o estado do aplicativo
 * @param[in] novo estado
 */
void ISR_escreveEstado (tipo_estado novo);

/*!
 * @brief Inicializa buffers circulares para o aplicativo
 */
void ISR_inicializaBC ();

/*!
 * @brief Extrai do buffer de entrada uma linha de caracteres.
 * @param[out] string contendo a linha terminada com '\0'.
 */
void ISR_extraiString (char *string);

/*!
 * @brief Envia 10 vezes uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 */
void ISR_EnviaString10x (char *string);

/*!
 * @brief Envia uma string para Terminal. A forma de onda eh espelhada no pino 2 do header H5
 * @param[in] string a ser enviado para Terminal via buffer circular
 */
void ISR_EnviaString (char *string);

/*!
 * @brief Envia caracteres de controle para realinhamento das linhas no Terminal
 */
void ISR_Realinhamento();

/*!
 * @brief Verificar se o buffer de saida esta vazio
 * @return estado vazio do buffer
 */
uint8_t ISR_BufferSaidaVazio ();

#endif /* ISR_H_ */
