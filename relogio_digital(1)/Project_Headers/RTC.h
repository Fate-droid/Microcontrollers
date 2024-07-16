/*
 * @file RTC.h
 * @brief Declaracao de prototipos, macros, tipos de dados relativos a RTC
 * @date Jan 16, 2023
 * @author Wu Shin-Ting
 */

#ifndef RTC_H_
#define RTC_H_

/**
 * @brief Inicializa RTC com LPO como a fonte de sinal de relogio.
 */
void RTClpo_init();

/**
 * @brief Le o tempo corrente em segundos
 * @param[out] seconds lidos
 * @return segundos lidos
 */
uint32_t RTClpo_getTime (uint32_t *seconds);

/**
 * @brief Configure o tempo corrente com segundos dados
 * @param[in] seconds a serem configurados em RTC
 */
void RTClpo_setTime (uint32_t seconds);

/**
 * @brief Habilita IRQ21 (eventos de segundos)
 */
void RTC_ativaSegundoIRQ (uint8_t priority);

/**
 * @brief Habilita IRQ20 (outros eventos de RTC)
 */
void RTC_ativaAlarmIRQ (uint8_t priority);

/******************************************************
 * 
 * Habilita pinos 1 e 2 de H5 para analisar os tempos em 1s e em um incremento de RTC_TSR
 * 
 ******************************************************/
/**
 * @brief Habilita PTE22 e PTE23 para monitorar o periodo e os "segundos" do RTC
 */
void GPIO_initH5P1P2();

/**
 * @brief Alterna o sinal em PTE23
 */
void GPIO_toggleH5P1();

#endif /* RTC_H_ */
