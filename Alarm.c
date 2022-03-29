#include "stm32l476xx.h"

#include "LCD.h"
#include "RTC.h"
#include "SysClock.h"
#include "Buzzer.h"

void RTC_Set_AlarmA(uint32_t Hour, uint32_t Minute, uint32_t Second) {
	//disable alarm A and alarm B
	RTC->CR &= ~(RTC_CR_ALRAE); 
	RTC->CR &= ~(RTC_CR_ALRBE); 
	
	//disable write protection
	RTC_Disable_Write_Protection(); 
	
	//clear interrupt for alarm A and B
	RTC->CR &= ~(RTC_CR_ALRAIE); 
	RTC->CR &= ~(RTC_CR_ALRBIE); 
	
	//wait until both access to alarm A is allowed
	while ((RTC->ISR & RTC_ISR_ALRAWF) == 0);
	
	//enable masks
	RTC->ALRMAR &= ~(RTC_ALRMAR_MSK3|RTC_ALRMAR_MSK2|RTC_ALRMAR_MSK1);
	
	//disable masks
	RTC->ALRMAR |= RTC_ALRMAR_MSK4;
	
	//set alarm time
	RTC->ALRMAR &= ~(RTC_ALRMAR_HT|RTC_ALRMAR_HU|RTC_ALRMAR_MNT|RTC_ALRMAR_MNU|RTC_ALRMAR_ST|RTC_ALRMAR_SU);
	RTC->ALRMAR |= 0x00000000|(Hour << 16)|(Minute << 8)|Second;

	//enable alarm A
	RTC->CR |= RTC_CR_ALRAE; //enable alarm A

	//enable interrupt for alarm A
	RTC->CR |= RTC_CR_ALRAIE;

	//enable write protection
	RTC_Enable_Write_Protection();
}

void RTC_Set_AlarmB(uint32_t Hour, uint32_t Minute, uint32_t Second) {
	//disable alarm B
	RTC->CR &= ~(RTC_CR_ALRBE); 
	
	//disable write protection
	RTC_Disable_Write_Protection(); 
	
	//clear interrupt for alarm B
	RTC->CR &= ~(RTC_CR_ALRBIE); 
	
	//wait until access to alarm B is allowed
	while ((RTC->ISR & RTC_ISR_ALRBWF) == 0);
	
	//enable masks
	RTC->ALRMBR &= ~(RTC_ALRMBR_MSK3|RTC_ALRMBR_MSK2|RTC_ALRMBR_MSK1);
	
	//disable masks
	RTC->ALRMBR |= RTC_ALRMBR_MSK4;
	
	//set alarm time
	RTC->ALRMBR &= ~(RTC_ALRMBR_HT|RTC_ALRMBR_HU|RTC_ALRMBR_MNT|RTC_ALRMBR_MNU|RTC_ALRMBR_ST|RTC_ALRMBR_SU);
	RTC->ALRMBR |= 0x00000000|(Hour << 16)|(Minute << 8)|Second;

	//enable alarm B
	RTC->CR |= RTC_CR_ALRBE;
	
	//enable interrupt for alarm B
	RTC->CR |= RTC_CR_ALRBIE; 
	
	//enable write protection
	RTC_Enable_Write_Protection();
}
void RTC_Alarm_Enable(void) {
	//configure as rising edge
	EXTI->RTSR1 |= EXTI_RTSR1_RT18; 
	
	//set interrupt mask
	EXTI->IMR1 |= EXTI_IMR1_IM18;
	
	//set event mask
	EXTI->EMR1 |= EXTI_EMR1_EM18;
	
	//clear the pending interrupt
	EXTI->PR1 |= EXTI_PR1_PIF18; 
	
	//enable and set priority
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(RTC_Alarm_IRQn, 1);
}

void RTC_Alarm_IRQHandler(void) {
	//alarms A and B interrupt handler
	if (RTC->ISR & RTC_ISR_ALRAF) {
		//clear pending bit
		EXTI->PR1 |= EXTI_PR1_PIF18;
		
		//turn on buzzer
		//while(1)
		//{
			Buzzer_ON();
			Buzzer_OFF();
			//delay_us(500000);
		//}
		
		//reset alarm event flag
		RTC->ISR &= ~(RTC_ISR_ALRAF); //reset alarm event flag
	}
	if (RTC->ISR & RTC_ISR_ALRBF) {
		//clear pending bit
		EXTI->PR1 |= EXTI_PR1_PIF18; 
		
		//turn on buzzer
		Buzzer_ON();
		Buzzer_OFF();
		
		//reset alarm event flag
		RTC->ISR &= ~(RTC_ISR_ALRBF);
	}
}