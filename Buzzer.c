#include "Buzzer.h"
#include "LCD.h"

void Buzzer_GPIO_Init(void) {
	//enable GPIO A clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//initialize PA10: Vdd
	GPIOA->MODER &= ~GPIO_MODER_MODE10;
	GPIOA->MODER |= GPIO_MODER_MODE10_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT10;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10;
}

void Buzzer_ON(void) {
	//turn on buzzer
	GPIOA->ODR |= GPIO_ODR_OD10;
	delay_us(5000000);
}

void Buzzer_OFF(void) {
	//turn off buzzer
	GPIOA->ODR &= ~GPIO_ODR_OD10;
	//delay_us(500000);
}
void Buzzer_ONOFF(void) {
	//turn buzzer on then off
	Buzzer_ON();
	Buzzer_OFF();
}

void Buzzer_TOGGLE(void) {
	//toggle buzzer
	GPIOA->ODR ^= GPIO_ODR_OD10;
	delay_us(500000);
}