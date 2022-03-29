#ifndef __STM32L476R_NUCLEO_UART_H
#define __STM32L476R_NUCLEO_UART_H

#include "stm32l476xx.h"
#include "Alarm.h"

#include <string.h>
#include <stdio.h>

#define BufferSize 32

void UART2_Init(void);
void UART2_GPIO_Init(void);

void USART_Init(USART_TypeDef* USARTx);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_Read(USART_TypeDef * USARTx);
void USART_Delay(uint32_t us);

void Init_USARTx(void);

uint32_t UART2_Set_Hour(int alarm);
uint32_t UART2_Set_Minute(int alarm);
uint32_t UART2_Set_Second(int alarm);

void setAlarm(void);

int getGlobalAlarm(void);
#endif
