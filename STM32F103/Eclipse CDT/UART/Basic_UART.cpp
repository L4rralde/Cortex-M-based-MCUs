#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
int i;
//USART1<<APB2<<48/72 MHz.
//USART2<<APB1<<24/36 MHz.
//TX1<<PA9
//RX1<<PA10

//For basic configuration only BRR (Baud Rate Register) and Control 1 Register
//are needed to setup the module.

//There are other Registers for more complicated configurations:
//			*Control Register 1: Enables TE, RE and USART (and others)
//			*Control Register 2: Stop bits and others
//			*Control Register 3: Interruptions configuration and others.

void USART_write(char ch);
void delayMs(int delay);

int main(void){
//Step 1: Enable the clocks (PORTA, USART1)
	RCC->APB2ENR |= 1<<14; //UART1EN : APB2ENR(14) = 1;
	RCC->APB2ENR |= 1<<2;  //GPIOAEN : APB2ENR(2) = 1;

//Step 2: Set up pins (PA9<=TX1: ; PA10<=RX1:)
//In this case, MUX the alternate function is not necessary at the port.
	//But, for more complex MCUs (such STM32F4) it is.
	//TX: Alternate function Push-pull
	//RX: input floating / input pull-up

	GPIOA->CRH |= 0x90;  //By the moment, only TX is configured.

//Step 3: Configure USART module:

	//USART1->BRR = 0x341; //52.0625 = 52;1=34;1 //Actual: 9604 bps @8MHz?
	USART1->BRR = 0x1D4C; //468.75;  //9600 @72MHz: By default is set to its maximum.
												//For the purpose of the misiion, we rather
												//Change this.
	USART1->CR1 |= 1<<3;    //Enable transmitter
	USART1->CR1 |= 1<<13;  	//Enable USART1 module


  while(1){
	  USART_write('H');
	  USART_write('o');
	  USART_write('l');
	  USART_write('i');
	  USART_write(13); //USART_write('\r')
	  USART_write(10); //USART_write('\n')
	  delayMs(1000);
    }
}

void USART_write(char ch){
	while(!(USART1->SR & 0x80)); //While not TXE: TXE=0 while data is not transferred to the shift register.
	USART1->DR = (ch & 0xFF);
}

void delayMs(int delay){
	for(; delay>0; delay--)
		for(i=0; i<1697; i++);
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
