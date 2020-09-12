#include "stm32f10x.h"

//Unfortunately, this project doesn't work as expected :c
//An oscilloscope would work or debugging.

//Actually it works very fine. It is a mater of noise.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void USART_write(char);

//For this project, I'm using TIM3 capture on channel 1 and compare on TIM2 channel 1.
//Channel 1 of TIM2 is connected to PA0.
//Channel 1 of TIM3 is connected to PA6.

//In order to know the captured value, I will send the value of the register
//to the computer via UART. So, I'm including those lines.

int main(void){
	//This is only for UART1. Erase if possible.
	RCC->APB2ENR |= 1<<14; //UART1EN : APB2ENR(14) = 1;
	RCC->APB2ENR |= 1<<2;  //GPIOAEN : APB2ENR(2) = 1;
	GPIOA->CRH |= 0x90;  //By the moment, only TX is configured.
	USART1->BRR = 0x1D4C; //468.75;  //9600 @72MHz: By default is set to its maximum.
	USART1->CR1 |= 1<<3;    //Enable transmitter
	USART1->CR1 |= 1<<13;  	//Enable USART1 module


	//Configure GPIOs for this project (no UART)
	RCC->APB2ENR |= 1<<2; //Enable GPIOA clock.
	GPIOA->CRL |= 0x9; //PA0 as alternate output push-pull.
	GPIOA->CRL |= 0x4000000; //Input floating (DEFAULT (e.g. 4)).

	//Configure TIM2
	RCC->APB1ENR |= 1; 		//Enable TIM2 Clock
	TIM2->PSC = 3600-1;		//TIM2 prescaler
	TIM2->ARR = 20000-1;	//TIM2 auto-reload value
	TIM2->CCMR1 = 0x30; 	//Configure Output to toggle.
	TIM2->CCR1 = 0; 		//Value to be compared
	TIM2->CCER |= 1;		//Enable this function
	TIM2->CNT = 0;



	//Configure TIM3
	RCC->APB1ENR |= 1<<1; 	//Enable TIM3 clock
	TIM3->PSC = 36000-1;
	//TIM3->ARR is not used because we need its maximum.
	TIM3->CCMR1 = 0x31; //Second nibble is to configure a digital filter.
						//First is to set as input and map it to TI1
	//Here, TIM3->CCR1 registers counter value captured.
	TIM3->CCER = 1; 	//Enable capture

	//Finally, I enabled both TIM2 and TIM3 counters.
	TIM2->CR1 = 1; //Enable TIM2 counter. Better put this at the end.
	TIM3->CR1 = 1;

	uint32_t we, aux;

	while(true){
		while(!(TIM3->SR & 2));		//Wait until a counter value has been captured.
							//This flag is cleared when TIMxCCR1 is read.
		we = TIM3->CCR1 ;
		//TIM3->SR &= ~0x2;
		USART_write('H');
		USART_write('\t');
		while(we>0){
			aux = we;
			we /= 10;
			USART_write(aux-we*10+48);
		}
		USART_write('\r');
		USART_write('\n');
	}

	return 0;
}

void USART_write(char ch){
	while(!(USART1->SR & 0x80)); //While not TXE: TXE=0 while data is not transferred to the shift register.
	USART1->DR = (ch & 0xFF);
}
#pragma GCC diagnostic pop


// ----------------------------------------------------------------------------
