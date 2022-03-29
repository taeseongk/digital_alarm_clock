#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H

#include "stm32l476xx.h"

void LCD_GPIO_Init(void); //initialize GPIO pins for LCD
void delay_us(uint32_t delay); //delay 
//void delay_ms(uint32_t delay);
static void lcd_send_8bit(uint8_t data); //send 8 bit data
static void lcd_send(int8_t rs,uint8_t data); //sends data or instruction
void lcd_cmd(uint8_t command); //send command
void lcd_data(char c); //send data
void lcd_init(void); //initialize lcd 
void lcd_clr(void);
void lcd_gotoxy(char x, char y);
void lcd_puts(char *text);

#endif