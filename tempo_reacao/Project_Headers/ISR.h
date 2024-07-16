/*
 * ISR.h
 *
 *  Created on: Feb 27, 2023
 *      Author: ea871
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag{
	PREPARA_INICIO,
	INICIO,
	PREPARA_AUDITIVO,
	ESPERA_ESTIMULO_AUDITIVO,
	ESPERA_REACAO_AUDITIVA,
	RESULTADO,
	LEITURA,
	ESPERA_LEITURA
} tipo_estado;

void ISR_EscreveEstado(tipo_estado valor);
tipo_estado ISR_LeEstado ();
void ISR_LeContagem (uint16_t *primeiro, uint16_t *segundo, uint8_t *counter);
void ISR_resetaTimeOut();
void ISR_escreveMultiplos(uint16_t valor);

#endif /* ISR_H_ */
