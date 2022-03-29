#include "LCD.h"

void LCD_GPIO_Init(void) {
	//enable GPIO A, B, C clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
	
	//initialize PA6: RS
	GPIOA->MODER &= ~GPIO_MODER_MODE6;
	GPIOA->MODER |= GPIO_MODER_MODE6_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;
	
	//initialize PA7: E
	GPIOA->MODER &= ~GPIO_MODER_MODE7;
	GPIOA->MODER |= GPIO_MODER_MODE7_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT7;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;
	
	//initialize PB6: D0
	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	GPIOB->MODER |= GPIO_MODER_MODE6_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
	
	//initialize PC7: D1
	GPIOC->MODER &= ~GPIO_MODER_MODE7;
	GPIOC->MODER |= GPIO_MODER_MODE7_0;
	GPIOC->OTYPER &= ~GPIO_OTYPER_OT7;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD7;
	
	//initialize PA9: D2
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->MODER |= GPIO_MODER_MODE9_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
	
	//initialize PA8: D3
	GPIOA->MODER &= ~GPIO_MODER_MODE8;
	GPIOA->MODER |= GPIO_MODER_MODE8_0;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT8;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD8;
	
	//initialize PB10: D4
	GPIOB->MODER &= ~GPIO_MODER_MODE10;
	GPIOB->MODER |= GPIO_MODER_MODE10_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT10;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD10;
	
	//initialize PB4: D5
	GPIOB->MODER &= ~GPIO_MODER_MODE4;
	GPIOB->MODER |= GPIO_MODER_MODE4_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT4;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;
	
	//initialize PB5: D6
	GPIOB->MODER &= ~GPIO_MODER_MODE5;
	GPIOB->MODER |= GPIO_MODER_MODE5_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT5;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5;
	
	//initialize PB3: D7
	GPIOB->MODER &= ~GPIO_MODER_MODE3;
	GPIOB->MODER |= GPIO_MODER_MODE3_0;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT3;
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3;
}

void delay_us(uint32_t delay){
	delay*=32;
	while(delay--);
}

void delay_1(uint32_t delay){
	delay *= 1000;
	delay*=32;
	while(delay--);
}

static void lcd_send_8bit(uint8_t data){	
	
	//send D0
	if(data & 0x01) {
		GPIOB->ODR |= GPIO_ODR_OD6;
	}
	else {
		GPIOB->ODR &= ~GPIO_ODR_OD6;
	}
	
	//send D1
	if(data & 0x02) {
		GPIOC->ODR |= GPIO_ODR_OD7;
	}
	else {
		GPIOC->ODR &= ~GPIO_ODR_OD7;
	}
	
	//send D2
	if(data & 0x04) {
		GPIOA->ODR |= GPIO_ODR_OD9;
	}
	else {
		GPIOA->ODR &= ~GPIO_ODR_OD9;
	}
	
	//send D3
	if(data & 0x08) {
		GPIOA->ODR |= GPIO_ODR_OD8;
	}
	else {
		GPIOA->ODR &= ~GPIO_ODR_OD8;
	}
	
	//send D4
	if(data & 0x10) {
		GPIOB->ODR |= GPIO_ODR_OD10;
	}
	else {
		GPIOB->ODR &= ~GPIO_ODR_OD10;
	}
	
	//send D5
	if(data & 0x20) {
		GPIOB->ODR |= GPIO_ODR_OD4;
	}
	else {
		GPIOB->ODR &= ~GPIO_ODR_OD4;
	}
	
	//send D6
	if(data & 0x40) {
		GPIOB->ODR |= GPIO_ODR_OD5;
	}
	else {
		GPIOB->ODR &= ~GPIO_ODR_OD5;
	}
	
	//send D7
	if(data & 0x80) {
		GPIOB->ODR |= GPIO_ODR_OD3;
	}
	else {
		GPIOB->ODR &= ~GPIO_ODR_OD3;
	}
}

static void lcd_send(int8_t rs,uint8_t data){
	//send RS
	if (!rs) {
		GPIOA->ODR &= ~GPIO_ODR_OD6;
	}
	else {
		GPIOA->ODR |= GPIO_ODR_OD6;
	}
	
	//send 8 bit data
	lcd_send_8bit(data);
	
	//enable, delay, disable
	GPIOA->ODR |= GPIO_ODR_OD7;
	delay_us(100);
	GPIOA->ODR &= ~GPIO_ODR_OD7;
}

void lcd_cmd(uint8_t command){
	//send command
	lcd_send(1,command);
}

void lcd_data(char c){
	//send data
	lcd_send(1,(uint8_t)c);
}

void lcd_init(void){
	//initialize LED by sending commands
	lcd_send(0,0x33);
  lcd_send(0,0x32);
  lcd_send(0,0x38);
  lcd_send(0,0x0C);
  lcd_send(0,0x06);
  lcd_send(0,0x01);
	delay_us(20000);
}

void lcd_clr(void){
	//clear display
  lcd_send(0,0x01);
  delay_us(5000);
}


void lcd_gotoxy(char x, char y){
	//select cursor
	if(x==0)	lcd_send(0,0x80+y);
	else if (x == 1) lcd_send(0,0xc0+y);
}

void lcd_puts(char *text){
	//send string
	while(*text){
		lcd_data(*text);
    text++;
  }
}
