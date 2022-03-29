#ifndef __STM32L476R_NUCLEO_BUTTON_H
#define __STM32L476R_NUCLEO_BUTTON_H

#include "stm32l476xx.h"
#include "Buzzer.h"
#include "UART.h"
#include "LCD.h"

void Button_GPIO_Init(void);
void EXTI_Init(void);
void EXTI2_IRQHandler(void);
void Green_LED_Toggle();
void LED_Init();

#endif
