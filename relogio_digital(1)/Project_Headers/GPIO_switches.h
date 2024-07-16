/*
 * @file GPIO_switchhes.h
 * @brief Prototipos, macros e tipos de dados de GPIO_switches
 * @date Jan 13, 2023
 * @author Wu Shin-Ting
 */

#ifndef GPIO_SWITCHES_H_
#define GPIO_SWITCHES_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief Inicializa botoeira switches NMI, IRQA5, e IRQA12.
 * @param[in] IRQC modo de interrupcao
 * @param[in] prioridade de atendimento
 */
void GPIO_initSwitches (uint8_t IRQC, uint8_t prioridade);


/**
 * @brief Desativa IRQ30, sem limpar as pendencias
 */
void GPIO_desativaSwitchesNVICInterrupt ();

/**
 * @brief Reativa IRQ30, sem limpar as pendencias
 */
void GPIO_reativaSwitchesNVICInterrupt ();

/**
 * @bief Desativa IRQC da botoeira IRQA5
 */
void GPIO_desativaSwitches();

/**
 * @brief Reativa IRQC da botoeira IRQA5 no modo de interrupcao especificado
 * @param[in] IRQA5_IRQC
 */
void GPIO_reativaSwitches (uint8_t IRQA5_IRQC);

#endif /* GPIO_SWITCHES_H_ */
