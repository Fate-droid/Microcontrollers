/*!
 * @file TPM.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo TPM.c
 * @author Wu Shin Ting
 * @date 03/03/2022
 */
//Ting: E o credito para voces? Nao sao co-autores do projeto?

#ifndef TPM_H_
#define TPM_H_

#include "stdint.h"
#include "util.h"


/**
 * @brief ...
 * @param[in] ...
 * @param[in] ...
 * @return
 */
uint16_t TPM_leChValor (uint8_t x, uint8_t n);

/**
 * @brief ...
 * @param[in] ...
 * @param[in] ...
 * @param[in] ...
 */
void TPM_atualizaDutyCycleEPWM (uint8_t x, uint8_t n, uint8_t percentagem);

/**
 * @brief ...
 * @param[in] ...
 * @param[in] ...
 * @param[in] ...
 */
void TPM_chaveiaChModoNivel (uint8_t x, uint8_t n, uint8_t MbaEba);

/**
 * @brief ...
 * @param[in] ...
 * @param[in] ...
 * @param[in] ...
 * @param[in] ...
 * 
 */
void TPM_initPTB0EPWM (uint16_t periodo, uint8_t ps, uint8_t percentagem, booleano_type mode);
/*!
 * @brief Inicializa pinos 1 (TPM2_CH1) e 2 (TPM2_CH0). 
 * @param[in] mod contagem maxima
 * @param[in] ps divisor de frequencia (prescaler)
 * @param[in] percentagem da contagem maxima
 * @param[in] mode polaridade
 * 
 * @note A base de tempo assincrono do contador eh MCGFLLCLK 
 * (Ver Secao 31.4.8.1 de KL25 Sub-Family Reference Manual)
 */
void TPM_initH5Pin12EPWM (uint16_t periodo, uint8_t ps, uint8_t percentagem, booleano_type modo);

/**
 * @brief Atualiza a largura do PWM no pino 2 do H5
 * @param[in] valor duty cycle
 */
void TPM_atualizaDutyCycleH5Pin2 (uint8_t valor);

/**
 * @brief Atualiza a largura do PWM no pino 1 do H5
 * @param[in] valor duty cycle
 */
void TPM_atualizaDutyCycleH5Pin1 (uint8_t valor);

/**
 * @brief Inicializa modulo TPM
 * @param[in] unidado do modulo
 * @param[in] periodo
 * @param[in] ps pre-scale
 */
void TPM_init (uint8_t x, uint16_t periodo, uint8_t ps);

/**
 * @brief Inicializa um canal n do TPMx com funcao EPWM
 * @param[in] x unidade de TPM
 * @param[in] n canal 
 * @param[in] percentagem duty cycle
 * @param[in] mode polaridade (ativo-alto ou ativo-baixo)
 */
void TPM_initChEPWM (uint8_t x, uint8_t n, uint8_t percentagem, booleano_type mode);

/**
 * @brief Atualiza a polaridade do PWM no canal do tpm especificados
 * @param[in] x unidade do tpm
 * @param[in] n canal
 * @param[in] mode polaridade (high-true/low-true pulses)
 */
void TPM_atualizaPolaridadeEPWM (uint8_t x, uint8_t n, booleano_type mode);

/**
 * @brief ???
 * @param[in] ...
 * @return 
 */
uint16_t TPM_leMOD (uint8_t x);

#endif /* TPM_H_ */
