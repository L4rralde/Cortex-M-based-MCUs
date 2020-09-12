#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//This project is almost the same that last one.
//However, now we try to measure signal's period.
//(maybe frequency too).

void UART_write(char);

int main(void){
//The configuration is almost the same.
	//Lines to config UART1
	RCC->APB2ENR |= 1<<14; 	//UART1EN : APB2ENR(14) = 1;
	RCC->APB2ENR |= 1<<2;  	//GPIOAEN : APB2ENR(2) = 1;
	GPIOA->CRH |= 0x90;  	//By the moment, only TX is configured.
	USART1->BRR = 0x1D4C; 	//468.75;  //9600 @72MHz: By default is set to its maximum.
	USART1->CR1 |= 1<<3;    //Enable transmitter
	USART1->CR1 |= 1<<13;  	//Enable USART1 module

	//Lines for the project (no UART)
	//Remembering: PA0 is TIM2:CH1 and PA6 is TIM3:CH1
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL |= 0x9;
	GPIOA->CRL |= 0x4000000;

	//Configure TIM2 for compare mode.
	RCC->APB1ENR |= 1;		//TIM2 clock enabled.
	TIM2->PSC = 36000-1; 	//Prescaler = 36000
	TIM2->ARR = 50-1;		//Set to fix toggle period (in ms)
	TIM2->CNT = 0; 			//Counter cleared.
	TIM2->CCR1 = 0; 		//Value to reach to match.
	TIM2->CCMR1 = 0x30; 	//Output toggles when mat
	TIM2->CCER = 1;


	//Configure TIM3 for input capture.
	RCC->APB1ENR |= 2;
	TIM3->PSC = 90-1;
	TIM3->CCMR1 = 0x51;
	TIM3->CCER = 0x3;

	TIM2->CR1 |= 1;
	TIM3->CR1 |= 1;

	//Some variables
	uint32_t lastTime{0}, currentTime, periodms;

	while(true){
		while(!(TIM3->SR & 2));
		currentTime = TIM3->CCR1;
		periodms = (currentTime-lastTime)/8;
		lastTime = currentTime;

		while(periodms>0){
			UART_write(48+periodms-(periodms/10)*10);
			periodms /= 10;
		}
		UART_write(10);
		UART_write(13);
	}
}

void UART_write(char ch){
	while(!(USART1->SR & 0x80)); //While not TXE: TXE=0 while data is not transferred to the shift register.
	USART1->DR = (ch & 0xFF);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
