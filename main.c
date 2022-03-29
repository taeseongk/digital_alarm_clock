#include "stm32l476xx.h"

#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include "LCD.h"
#include "RTC.h"
#include "Buzzer.h"
#include "Alarm.h"
#include "Button.h"
#include <string.h>

int main(void) {
	//string buffers
	char strTime[12] = {0};
	char strDate[12] = {0};
	char temp[12] = {0};

	//initialize clock
	System_Clock_Init();
	
	//initialize LCD
	LCD_GPIO_Init();
	lcd_init();
	
	//initialize UART
	Init_USARTx();
	
	//initialize RTC
	RTC_Init();
	
	//initialize RTC alarms
	RTC_Alarm_Enable();
	
	//set Alarm
	setAlarm();
	
	//initialize buzzer
	Buzzer_GPIO_Init();
	
	//initialize button
	EXTI_Init();
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();
	
	uint8_t SlaveAddress;
	uint8_t Data_Receive;
	uint8_t Data_Send;
	
	while(1) {
		SlaveAddress = 0b1001000 << 1;
		Data_Send = 0x00U;
		
		I2C_SendData(I2C1, SlaveAddress, &Data_Send, 1);
		I2C_ReceiveData(I2C1, SlaveAddress, &Data_Receive, 1);
		
		//printf("%d", Data_Receive);
		

		Get_RTC_Calendar(strTime, strDate);
		strcat(strTime, "   ");
		sprintf(temp, "%d C", Data_Receive);
		strcat(strTime, temp);
		//printf(strTime);
		lcd_gotoxy(0,0);
		lcd_puts(strTime);
		lcd_gotoxy(1,0);
		lcd_puts(strDate);
	}
}
