#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(void){
	__disable_irq();
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	RCC->APB1ENR |= 1;
	TIM2->CNT = 0; 			//Clears TIM2 counter.
	TIM2->PSC = 7200-1;		//Prescaler of 7200
	TIM2->ARR = 10000-1;	//Counts up to 10000. (1 Hz with prescaler)
	TIM2->CR1 |= 1; 		//Enable counter

	TIM2->DIER |= 1; 	//Update interrupt enabled

	//TIM2 counters from ARR to 0. When it reaches 0 it is autoreloaded with ARR.
	//TIM2_DIER(0)=1 enables an interruption when counter is updated.

	//TIMx_DIER has other bits to enabled other interruptiosn related with GP Timers.
	//CCapture Compare, for example, is another interruption that can be trigger with a GP Timer.

	NVIC_EnableIRQ(TIM2_IRQn);
	__enable_irq();

	while(1){

	}
	return 0;
}

void TIM2_IRQHandler(void){
	if(TIM2->SR & 0x1){
		GPIOC->ODR ^= 0x2000;
		TIM2->SR &= ~0x1;
	}
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
