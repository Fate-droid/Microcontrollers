/*!
 * @file TPM.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo TPM.c
 * @author Fabio e Felipe
 * @date 08/11/2022
 */

#ifndef TPM_H_
#define TPM_H_

#include "stdint.h"
#include "util.h"


/**
 * @brief Inicializa modulo TPM
 * @param[in] unidado do modulo
 * @param[in] periodo
 * @param[in] ps pre-scale
 */
void TPM_init (uint8_t x, uint16_t periodo, uint8_t ps);

/*!
 * @brief : retornar o valor do contador TPM[x] para CNT
 * 
 */

uint16_t TPM_LeCNT (uint8_t x);

/*!
 * @brief : ativa o canal n de TPMx para Input Capture, com mode especificado
 * 
 */


void TPM_ativaChIC (uint8_t x, uint8_t n, uint8_t mode);
/*!
 * @brief : ativa o canal n de TPMx para Output Compare, com mode especificado e valor a ser comparado.
 * 
 */

void TPM_ativaChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor);

/*!
 * @brief : desativa o canal n de TPMx
 */


void TPM_desativaCh (uint8_t x, uint8_t n);
/*!
 * @brief : ativa a interrupcao do canal n de TPMx
 */


void TPM_ativaChInterrupt (uint8_t x, uint8_t n);

/*!
 * @brief : desativa a interrupcao do canal n de TPMx
 */
void TPM_desativaChInterrupt (uint8_t x, uint8_t n);


/*!
 * @brief conectar as
botoeiras NMI, IRQA5 e IRQA12 nos canais TPM0_CH1, TPM0_CH2 e TPM1_CH0 cujos
contadores são configurados com TPMx_MOD = periodo e TPMx_SC_PS = ps
 * @param[in] periodo
 * @param[in] ps divisor de frequencia (prescaler)
 */
void TPM_initSwitchesIC (uint32_t periodo, uint8_t ps);


/*!
 * @brief : conectar os
leds R e G nos canais TPM2_CH0 e TPM2_CH1 cujo contador é configurado com
TPMx_MOD=periodo e TPMx_SC_PS=ps.
 * @param[in] periodo
 * @param[in] ps divisor de frequencia (prescaler)
 */
void TPM_initPTB18PTB19OC (uint32_t periodo, uint8_t ps);


/*!
 * @brief : cconfigurar NVIC para
habilitar a interrupção (Input Capture) dos canais em que as botoeiras são conectadas com os canais
desativados
 * @param[in] prioridade
 */
void TPM_habilitaSwitchesInterrupt (char priority);


/*!
 * @brief : configurar
NVIC para habilitar a interrupção (Output Compare) dos canais em que os leds R e G são
conectados com os canais desativados.
 * @param[in] prioridade
 */
void TPM_habilitTPM2CH0TPM2CH1Interrupt (char priority);


///*!
// * @brief :ativar o canal de NMI para IC (borda de
//descida) e a interrupção do canal.
// */
//
void TPM_ativaSwitchNMIInterrupt ();


///*!
// * @brief :desativar o canal de NMI
//(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
// */
void TPM_desativaSwitchNMIInterrupt ();


///*!
// * @brief :: ativar o canal de IRQA5 para IC (borda de
//descida) e a interrupção do canal.
// */
void TPM_ativaSwitchIRQA5Interrupt ();


///*!
// * @brief : desativar o canal de IRQA5
//(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
// */
//
void TPM_desativaSwitchIRQA5Interrupt ();


///*!
// * @brief : ativar o canal de IRQA12 para IC (borda
//de descida) e a interrupção do canal.
// */
void TPM_ativaSwitchIRQA12Interrupt ();


///*!
// * @brief : desativar o canal de IRQA12
//(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
// */
void TPM_desativaSwitchIRQA12Interrupt ();


///*!
// * @brief : ativar o canal de led R para OC (Clear Output on
//match) e a interrupção do canal.
// */
void TPM_ativaLedRInterrupt ();


///*!
// * @brief : desativar o canal de led R
//(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
// * 
// */
//
void TPM_desativaLedRInterrupt ();


///*!
// * @brief : ativar o canal de led G para OC,
//cujo modo pode ser Clear Output on match (0b10) ou Set Output on match (0b11) e a interrupção
//do canal.
// * @param[in] modo a ser ativado "Clear (0b10)ou set (0b11) output on match"
// */
//
//
void TPM_ativaLedGInterrupt (uint8_t modo);


///*!
// * @brief : desativar o canal de led G
//(MsnB:MsnA:ELSnB:ELSnA = 0b0000) e a interrupção do canal.
// * 
// */
void TPM_desativaLedGInterrupt ();

//
//
//
//
//
//
///*!
// * @brief Inicializa pinos 1 (TPM2_CH1) e 2 (TPM2_CH0) do header H5. 
// * @param[in] mod contagem maxima
// * @param[in] ps divisor de frequencia (prescaler)
// * @param[in] percentagem da contagem maxima
// * @param[in] mode polaridade
// * 
// * @note A base de tempo assincrono do contador eh MCGFLLCLK 
// * (Ver Secao 31.4.8.1 de KL25 Sub-Family Reference Manual)
// */
//void TPM_initH5Pin12EPWM (uint16_t periodo, uint8_t ps, uint8_t percentagem, booleano_type modo);
//
//
///*!
// * @brief Inicializa pino 3 (TPM1_CH1) no modo Output Compare. 
// * @param[in] mod contagem maxima
// * @param[in] ps divisor de frequencia (prescaler)
// */
//void TPM_initH5Pin3OC (uint32_t periodo, uint8_t ps);
//
///**
// * @brief Atualiza a largura do PWM no pino 2 do H5
// * @param[in] valor duty cycle
// */
//void TPM_atualizaDutyCycleH5Pin2 (uint8_t valor);
//
///**
// * @brief Atualiza a largura do PWM no pino 1 do H5
// * @param[in] valor duty cycle
// */
//void TPM_atualizaDutyCycleH5Pin1 (uint8_t valor);
//
/**
 * @brief Inicializa modulo TPM
 * @param[in] unidado do modulo
 * @param[in] periodo
 * @param[in] ps pre-scale
 */
void TPM_init (uint8_t x, uint16_t periodo, uint8_t ps);

///**
// * @brief Inicializa um canal n do TPMx com funcao EPWM
// * @param[in] x unidade de TPM
// * @param[in] n canal 
// * @param[in] percentagem duty cycle
// * @param[in] mode polaridade (ativo-alto ou ativo-baixo)
// */
//void TPM_initChEPWM (uint8_t x, uint8_t n, uint8_t percentagem, booleano_type mode);
//
///**
// * @brief Atualiza a polaridade do PWM no canal do tpm especificados
// * @param[in] x unidade do tpm
// * @param[in] n canal
// * @param[in] mode polaridade (high-true/low-true pulses)
// */
//void TPM_atualizaPolaridadeEPWM (uint8_t x, uint8_t n, booleano_type mode);
//
///*!
// * @brief Configura o temporizador TPM0 com um valor MOD. 
// * @param[in] mod contagem maxima
// * @param[in] ps divisor de frequencia (prescaler)
// * 
// * @note A base de tempo assincrono do contador eh MCGFLLCLK 
// * (Ver Secao 31.4.8.1 de KL25 Sub-Family Reference Manual)
// */
//void TPM_initSwitchNMIChannelTPM0CH2 (uint32_t mod, uint8_t ps);
//
//
///*!
// * @brief Configura o canal n do temporizador TPMx em IC. 
// * @param[in] x modulo TPM
// * @param[in] n canal
// * @param[in] mode modo de operacao em codigo binario (borda de subida, descida ou ambas)
// */ 
//void TPM_initChIC (uint8_t x, uint8_t n, uint8_t mode);
//
///*!
// * @brief Ativa/Inicializa o canal n do temporizador TPMx em OC. 
// * @param[in] x modulo TPM
// * @param[in] n canal
// * @param[in] mode modo de operacao em codigo binario (alternado, 0 ou 1)
// * @param[in] valor valor de comparacao
// */ 
//void TPM_initChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor);
//
///**
// * @brief Habilita IRQ do par (NMI,TPM0CH2) no NVIC e setar prioridade 
// * @param[in] priority prioridade de atendimento
// */
//void TPM_habilitaSwitchNMIChannelTPM0CH2Interrupt (char priority);
//
///**
// * @brief Ativa a interrupcao da chave NMI
// */
//void TPM_ativaSwitchNMIInterrupt ();
//
///**
// * @brief Desativa a interrupcao da chave NMI
// */
//void TPM_desativaSwitchNMIInterrupt ();
//
///**
// * @brief Ativa a interrupcao do canal TPM0CH2
// */
//void TPM_ativaChannelTPM0CH2Interrupt ();
//
///**
// * @brief Desativa a interrupcao do canal TPM0CH2
// */
//void TPM_desativaChannelTPM0CH2Interrupt ();
//
///**
// * @brief Desativa o canal TPMxCHn
// */
//void TPM_desativaCh (uint8_t x, uint8_t n);



/*!
 * @brief Configura o canal n do temporizador TPMx em IC. 
 * @param[in] x modulo TPM
 * @param[in] n canal
 * @param[in] mode modo de operacao em codigo binario (borda de subida, descida ou ambas)
 */ 
void TPM_initChIC (uint8_t x, uint8_t n, uint8_t mode);

/*!
 * @brief Ativa/Inicializa o canal n do temporizador TPMx em OC. 
 * @param[in] x modulo TPM
 * @param[in] n canal
 * @param[in] mode modo de operacao em codigo binario (alternado, 0 ou 1)
 * @param[in] valor valor de comparacao
 */ 
void TPM_initChOC (uint8_t x, uint8_t n, uint8_t mode, uint16_t valor);



#endif /* TPM_H_ */
