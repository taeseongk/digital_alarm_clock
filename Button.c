#include "Button.h"
int count = 1;
void EXTI_Init(void) {
	//initialize user button
	GPIOC->MODER &= ~(GPIO_MODER_MODE13); //Set mode of pin PC13 to input
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13); //Set PC13 to no pull-up and no pull-down
	
	//configure SYSCFG EXTI
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	//configure EXTI Trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT13; //we want to enable the falling edge trigger to 1.
	
	//enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13; //we want to enable this to 1 
	
	//configure and enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

void EXTI15_10_IRQHandler(void) {
	//clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	
	//buzzer off
	Buzzer_OFF();
}
