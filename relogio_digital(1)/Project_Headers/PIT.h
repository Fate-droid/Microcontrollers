/*
 * @file PIT.h
 * @brief Declaracao de prototipos, macros e tipos de dados relacionados com PIT
 * @date Jan 16, 2023
 * @autor Wu Shin-Ting
 */

#ifndef PIT_H_
#define PIT_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief Inicializa timer 0 do PIT
 * @param[in] periodo
 * @param[in] prioridade
 */
void PIT_initTimer0(uint32_t periodo, uint8_t prioridade);

/**
 * @brief Ativa IRQ no NVIC
 * @param[in] prioridade
 */
void PIT_ativaNVICIRQ (uint8_t priority);

/**
 * @brief Desabilita solicitacao IRQ do PIT
 */
void PIT_desabilitaIRQ ();

/**
 * @brief Reabilita solicitacao IRQ do PIT
 */
void PIT_reabilitaIRQ ();

/**
 * @brief Atualiza o valor maximo de contagem
 */
void PIT_setaLDVAL (uint32_t periodo);

/**
 * @brief Desativa timer0
 */
void PIT_desativaTimer0 ();

/**
 * @brief Ativa Timer0
 */
void PIT_ativaTimer0 ();

/******************************************************
 * 
 * Habilita pino 3 de H5 para analisar os sinais 
 * 
 ******************************************************/
/**
 * @brief HHabilita PTE21 para monitorar periodos configurados para PIT
 */
void GPIO_initH5P3P4();

#endif /* PIT_H_ */
