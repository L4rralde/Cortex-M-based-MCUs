#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//Let's configure TIM2:CH1 (PA0) for a PWM signal.

int main(void){
	//Configure PA0 as alternate fucntion push-pull
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL |= 9;

	//Configure TIM2
	RCC->APB1ENR |= 1;
	TIM2->PSC = 72-1;
	TIM2->ARR = 2400-1;
	TIM2->CNT = 0;
	TIM2->CCMR1 = 0x60; 	//Configure on PWM mode 1. 0x70 is PWM mode 2.
							//PWM mode 1 sets the output as long as
							//counter is less than CCR1
	TIM2->CCR1 = 2200-1; 	//Pulse width.
	TIM2->CCER |= 1;
	TIM2->CR1 |= 1;

	//So, this configuration sets TIM2:CH1 as a PWM with a frequency of SYSCLK/((ARR+1)*(prescaler+1))
	//and duty cycle of (CCR1+1)/(ARR + 1).

	while(true){

	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
