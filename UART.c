#include "UART.h"

#define __RTC_CONVERT_BIN2BCD(__VALUE__) (uint8_t)((((__VALUE__) / 10) << 4) | ((__VALUE__) % 10))
#define __RTC_CONVERT_BCD2BIN(__VALUE__) (uint8_t)(((uint8_t)((__VALUE__) & (uint8_t)0xF0) >> (uint8_t)0x4) * 10 + ((__VALUE__) & (uint8_t)0x0F))
int globalAlarm;
void UART2_Init(void) {
	//enable USART2 clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	
	//select system clock as USART2 clock source
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART2_GPIO_Init(void) {
	//set up PA2
	
	//enable GPIOA clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//set mode to alt function
	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	
	//set to AF7
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2;
	
	//set very high speed
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED2);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;
	
	//set push-pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT2);
	
	//set pull-up 
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;
	
	//set up PA3
	
	//set mode to alt function
	GPIOA->MODER &= ~(GPIO_MODER_MODE3);
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
	//set to AF7
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL3);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2;
	
	//set very high speed
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED3);
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
	
	//set push-pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT3);
	
	//set pull-up
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD3);
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	//disable USART
	USARTx->CR1 &= ~(USART_CR1_UE);
	
	//set word length to 8 bits
	USARTx->CR1 &= ~(USART_CR1_M);
	
	//set oversample by 16
	USARTx->CR1 &= ~(USART_CR1_OVER8);
	
	//set stop bits to 1
	USARTx->CR2 &= ~(USART_CR2_STOP);
	
	//set baud rate to 9600
	USARTx->BRR &= ~(USART_BRR_DIV_FRACTION | USART_BRR_DIV_MANTISSA);
	USARTx->BRR |= 8333UL;
	
	//enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE;
	USARTx->CR1 |= USART_CR1_RE;
	
	//enable USART
	USARTx->CR1 |= USART_CR1_UE;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

void Init_USARTx() {
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);
}

uint32_t UART2_Set_Hour(int alarm) {
	int hour;
	if (alarm == 1) printf("Set hour for Alarm A: ");
	else if (alarm == 2) printf("Set hour for Alarm B: ");
	scanf("%d", &hour);
	return __RTC_CONVERT_BIN2BCD(hour);
}

uint32_t UART2_Set_Minute(int alarm) {
	int minute;
	if (alarm == 1) printf("Set minutes for Alarm A: ");
	else if (alarm == 2) printf("Set minutes for Alarm B: ");
	scanf("%d", &minute);
	return __RTC_CONVERT_BIN2BCD(minute); 
}

uint32_t UART2_Set_Second(int alarm) {
	int second;
	if (alarm == 1) printf("Set seconds for Alarm A: ");
	else if (alarm == 2) printf("Set seconds for Alarm B: ");
	scanf("%d", &second);
	return __RTC_CONVERT_BIN2BCD(second); 
}

void setAlarm() {
	char rxByte;
	int check0 = 1;
	do {
		printf("Set Alarm? (Y for yes, N for no): ");
		scanf("%c", &rxByte);
		
		if(rxByte == 'Y' || rxByte == 'y'){
			check0 = 0;
			int check = 1;
			int a;
			char alarm;
			uint32_t hour;
			uint32_t minute;
			uint32_t second;
			do {
				printf("Alarm A or B? ");
				scanf("%c", &alarm);
				if (alarm == 'A') {
					globalAlarm = 1;
					check = 0;
					a = 1;
					hour = UART2_Set_Hour(1);
					minute = UART2_Set_Minute(1);
					second = UART2_Set_Second(1);
				}
				else if (alarm == 'B') {
					globalAlarm = 2;
					check = 0;
					a = 2;
					hour = UART2_Set_Hour(2);
					minute = UART2_Set_Minute(2);
					second = UART2_Set_Second(2);
				}
			} while (check);
			if (a == 1) {
				RTC_Set_AlarmA(hour, minute, second);
				printf("Set Alarm A! \n");
			}
			else if (a== 2) {
				RTC_Set_AlarmB(hour, minute, second);
				printf("Set Alarm B! \n");
			}
		} 
		else if(rxByte == 'N' || rxByte == 'n'){
			printf("Done! \n");
			return;
		} 
	} while (check0);
	
}
