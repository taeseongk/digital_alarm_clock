#ifndef __STM32L476R_NUCLEO_BUZZER_H
#define __STM32L476R_NUCLEO_BUZZER_H

#include "stm32l476xx.h"

void Buzzer_GPIO_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_ONOFF(void);
void Buzzer_TOGGLE(void);

#endif