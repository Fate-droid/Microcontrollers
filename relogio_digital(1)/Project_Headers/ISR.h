/*
 * @file ISR.h
 * @brief Declaracao de prototipos, macros e tipos de dados relativos a rotinas de servico
 * @date Jan 16, 2023
 * @author Wu Shin-Ting
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag {
	NORMAL,
	INCREMENTE_HORA,
	INCREMENTE_HORA_ESPERA,
	INCREMENTE_MINUTO,
	INCREMENTE_MINUTO_ESPERA,
	INCREMENTE_SEGUNDO,
	INCREMENTE_SEGUNDO_ESPERA,
} tipo_estado;

#define POSTSCALER 1

/**
 * @brief Carrega vetor de horarios com o valor de tempo cronometrado em RTC.
 */
void ISR_carregaHorario();

/**
 * @brief Le horario do vetor de horarios
 * @param[out] dias
 * @param[out] horas
 * @param[out] minutos
 * @param[out] segundos
 * 
 */
void ISR_leHorario (uint32_t *dias, uint32_t *horas, uint32_t *minutos, uint32_t *segundos);

/**
 * @brief Le o estado do sistema
 */
tipo_estado ISR_LeEstado ();

/**
 * @brief atualiza o estado do sistema
 */
void ISR_escreveEstado (tipo_estado s);

#endif /* ISR_H_ */
