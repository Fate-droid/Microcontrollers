/*
 * GPIO_Switches.h
 * @brief Funcoes relacionadas as botoeiras (incializcao, determinar quais estao pressionadas)
 *  Created on: Oct 5, 2022
 *      Author: Fabio e Felipe
 */

#ifndef GPIO_SWITCHES_H_
#define GPIO_SWITCHES_H_
/**
 * @brief Inicializa as botoeiras
 */
void GPIOinitSwitches();
/**
 * @brief amostra quais botoeiras estão pressionadas usando o "valor"
 * @param[in] valor (usado para determinar botoeiras pressionadas)
 */
void GPIO_amostraSwitches(uint32_t *valor);
/**
 * @brief habilita as interrupcoes das botoeiras
 * @param[in] priority (prioridade da interrupcao)
 */
void GPIO_habilitaSwitchesInterrupt(uint8_t priority);
/**
 * @brief  mostra quais botoeiras estão pressionadas usando o "valor"
 * @param[in] valor (usado para determinar botoeiras pressionadas)
 */
void GPIO_leSwitchesISF (uint32_t *valor);
/**
 * @brief desativa as interrupcoes das botoeiras
 */
void GPIO_desativaSwitchesInterrupt ();
/**
 * @brief reativa as interrupcoes das botoeiras
 */
void GPIO_reativaSwitchesInterrupt ();


#endif /* GPIO_SWITCHES_H_ */
