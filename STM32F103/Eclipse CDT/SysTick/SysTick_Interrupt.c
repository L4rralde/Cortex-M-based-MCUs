#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(){
	__disable_irq();

	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	SysTick->LOAD = 7200000-1;	//It takes 0.1 second to count this
	//SysTick->LOAD = 0xFFFFFF;  	//There are only 24 bits available for writing.

	SysTick->VAL = 0; 	//Clears counter
	SysTick->CTRL = 1<<2; 	//System Clk is its source.
	SysTick->CTRL |= 1<<1; 	//Interruption enabled (when it counts to 0)
	SysTick->CTRL |= 1; 	//SysTick enabled
	//SysTick->CTRL = 7;

	//As this is part of the cortex, NVIC is not used.
	__enable_irq();
	while(1){

	}
	return 0;
}

void SysTick_Handler(void){
	GPIOC->ODR ^= 0x2000;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
