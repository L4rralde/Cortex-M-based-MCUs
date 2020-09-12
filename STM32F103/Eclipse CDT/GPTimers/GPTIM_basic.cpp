#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(void){
	//This program is to configure a General Purpose Timer...
	//...and use it for a led blink of 1 Hz.
	//This uC has 4 timers. TIM1 is an advance timer whilst...
	//...TIM2:4 are general purpose.

	//I start  setting up the port for the led
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	//Then, I configure a timer (TIM2)
	//Step 1: Enable the clock source
	RCC->APB1ENR |= 1; //APB1ENR[0] = TIM2EN

	//Every GPTIM has this 3 registers (in order to configure the timer):
		//TIMx_CNT: Counter Register
		//TIMx_PSC: Prescaler Register
		//TIMx_ARR: Auto-Reload Register
	//So, in order to get 1 Hz, I prescaler is input clock,...
	//... set its reload value and wail until the counter ...
	//... reaches a given value (e.g. 0).

	//Step 2: Prescaler
	TIM2->PSC = 7200 - 1;  //TIM2:4 are interfaced with APB1.
							//APB1 max frequency is 36 Mhz.
							//But it seems that every peripheral is
							//driven by SYSCLK, which its maximum is 72 MHz.

							//Hence, the timer frequency is 72MHz/7200 = 10 kHz.

	//Step 3: Initialized the counter
	TIM2->CNT = 0;

	//Step 4: Set the Auto-reload Register.
	TIM2->ARR = 10000 - 1; 	//know, it counst from 10000-1 to 0.
							//It takes 10000 cycles to reload, so ...
							//... the TIM2 counts with a reload rate of 1 Hz.

	//Step 5: Enable the counter.
	TIM2->CR1 = 1;  //TIM2_CR1(0) is counter enable bit.

	while(true){
		while((TIM2->SR & 1) == 0); //TIMx_SR(0) = 1 when its content has been reloaded.
		TIM2->SR &= ~1; //It's clear by software.
		GPIOC->ODR ^= 0x2000;
	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
