#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

const char fsys = 72;

void SysTick_delayMs(int );


int main(void ){
	//For this program I'll declare a function of ms delays
	//within SyStick configuration.

	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	while(true){
		GPIOC->ODR ^= 0x2000;
		SysTick_delayMs(500);
	}

	return 0;
}

void SysTick_delayMs(int n){
	SysTick->LOAD = fsys*1000;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x5;
	for(int i=0; i<n; i++)
		while((SysTick->CTRL & 0x10000) == 0);
	SysTick->CTRL = 0;
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
