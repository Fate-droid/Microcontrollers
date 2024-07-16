/**
 * @file GPIO_ledRGB.h
 * @brief Programas relacionados com a inicialização e uso do ledRGB.
 * @author Fabio Bareiro Fleitas
 * @date 05/10/22
 */

#ifndef GPIO_LEDRGB_H_
#define GPIO_LEDRGB_H_

#include "stdint.h"

int GPIO_initledRGB(void);

void GPIO_ledRGB(uint8_t estadoR,uint8_t estadoG,uint8_t estadoB);

void ativa_cor(uint8_t codigo_cor);

#endif /* GPIO_LEDRGB_H_ */
