#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(void){
	//For this project i will set one GPTim in compare mode
	//to drive one output (Make it toggle at a given frequency
	//and duty cycle (50%))

	//1. The GPIO need to be configure to work for this alternate function.
	//We need first to look what GPIO pins can be driven by the timer.
	//GPIO neeDs to be configured as alternate function push-pull

	//Datasheet says that TIM2 CH1 is connected to PA0.
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL |= 0x9; //PA0 as alternate function push-pull


	//2. Configure TIMER in compare mode

	//And TIM2 is the one
	RCC->APB1ENR |= 1; //Enable TIM2 clock.
	TIM2->PSC = 7200-1; //Prescaler
	TIM2->ARR = 10000-1; //Counts 1000
	TIM2->CCMR1 = 0x30; //Output toggles on match
	TIM2->CCR1 = 8000; //Value to be compared with the counter.
	TIM2->CCER |= 1; // signal (tim2:ch1) is output on the corresponding output pin.
	TIM2->CNT = 0; //Clear the counter.
	TIM2->CR1 = 1; //Enable TIM2

	while(true){

	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
