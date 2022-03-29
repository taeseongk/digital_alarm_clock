#ifndef __STM32L476R_NUCLEO_ALARM_H
#define __STM32L476R_NUCLEO_ALARM_H

#include "stm32l476xx.h"

void RTC_Set_AlarmA(uint32_t Hour, uint32_t Minute, uint32_t Second);
void RTC_Set_AlarmB(uint32_t Hour, uint32_t Minute, uint32_t Second);
void RTC_Alarm_Enable(void);
void RTC_Alarm_IRQHandler(void);

#endif
