#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


//This code configurates 2 external interruptions:
//	*PB11
//  *PA7

//When PB11 is being released (falling edge), soldered led (PC3) will togle
//When PA7 is in rising edge, the same LED will blink twice.

//In the main code, a LED (PA0) is blinking forever
int main(void){
	//Pins configuration
	__disable_irq();

	RCC->APB2ENR |= 1<<2; 	//PORTA clock enabled
	RCC->APB2ENR |= 1<<3; 	//PORTB clock enabled
	RCC->APB2ENR |= 1<<4; 	//PORTC clock enabled

	GPIOC->CRH |= 0x100000; //PC13 configured as PP output
	GPIOA->CRL |= 1;
	//Other pins as floaitng input. It's its default configuration.


	//Config interruptions
	RCC->APB2ENR |= 1; 	//AFIO clock enabled

	//PA7 first
	AFIO->EXTICR[1] |= 0;	//A is default
	EXTI->IMR |= 1<<7; 		//Line 7 unmasked
	EXTI->RTSR |= 1<<7; 	//Interrupt at rising edge of line 7.

	NVIC_EnableIRQ(EXTI9_5_IRQn);

	//Then, PB11
	AFIO->EXTICR[2] |= 0x1000; 	//Line 11 on PORTB
	EXTI->IMR |= 1<<11;
	EXTI->FTSR |= 1<<11;

	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();

	while(1){
		GPIOC->ODR ^= 0x2000;
		for(int i=0; i<1000000; i++);
	}
	return 0;
}

void EXTI9_5_IRQHandler(void){
	if(EXTI->PR & 1<<7){
		GPIOA->ODR ^= 1;
		for(int i=0; i<1000000; i++);
		GPIOA->ODR ^= 1;
		for(int i=0; i<1000000; i++);
		EXTI->PR |= 1<<7;
	}
}

void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & 1<<11){
		GPIOA->ODR ^= 1;
		for(int i=0; i<2000000; i++);
		EXTI->PR |= 1<<11;
	}
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
