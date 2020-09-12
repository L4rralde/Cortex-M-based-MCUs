#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int i;
//char byte;
void delayMs(int delay);
char UART_read(void);

int main(void){
//In this case I will config the MCU to read data from RX1 at 9600 bps.
//Also, we will use the messages to toggle a LED.
//Remember, RX1: PA9, Clocl tree is set up at is maximum frequencies and PC13...
//...is connected to the LED on board.

//Step 1: Enable clock
	RCC->APB2ENR |= 1<<2; 	//PORTA APB2 enabled
	RCC->APB2ENR |= 1<<14; 	//USART1 APB2 enabled
	RCC->APB2ENR |= 1<<4;	//PORTC APB2 enabled

//Step 2: Configure Terminals
	GPIOA->CRH |= 0x400; 	//PA10 as floating Input
	GPIOC->CRH |= 0x100000; //PC13 as push-pull output

//Step 3: Configure USART1
	USART1->BRR |= 0x271;  	//115200 bps @72MHz
	USART1->CR1 |= 1<<2; 	//RX enabled
	USART1->CR1 |= 1<<13;   //USART1 module enabled

//Some variables that are needed
char data;
	while(1){
		data = UART_read();
		if(data=='S'){
			GPIOC->BSRR |= 0x20000000;
		}else if(data=='N'){
			GPIOC->BSRR |= 0x2000;
		}
    }
}

void delayMs(int delay){
	for(; delay>0; delay--) for(i=0; i<1697; i++);
}

char UART_read(void){
	while(!(USART1->SR & 0x20)); 	//Wait until RXNE=1
									//RXNE=0 when it's no data received
	return USART1->DR;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
