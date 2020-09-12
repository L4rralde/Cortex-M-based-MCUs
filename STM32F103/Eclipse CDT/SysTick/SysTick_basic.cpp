#include "stm32f10x.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(void){
	RCC->APB2ENR |= 1 << 4;
	GPIOC->CRH |= 0x300000;

	SysTick->LOAD = 7200000-1; 	//For this MCU, SysTick CLK = AHB
								//So, delay = 7200000/72MHz = .1 s

	//Load is a 24 bits register. The largest value is 2^24-1= 16,777,215

	SysTick->VAL = 0; 		//Current Value = 0. Next clock
							//the value will be set to 7200000- 1;

	SysTick->CTRL = 5; 		//SysTick(0) = 1 => enable
							//SysTick(2) = 1 => clk soruce <= SysTick clk

	//It seems that the systick actually is kinda faster than I expected.
	//Maybe it actually its frequency is equal than AHB = 72 MHz. Hence,
	//this frequency is impossible to reach delays grater than 2^24/72MHz = 0.233 s.
	while(1){
		if(SysTick->CTRL & 0x10000) GPIOC->ODR ^= 0x2000;
			//while(SysTick->CTRL & 0x10000);
		//for(i =0; i< 1000000; i++);
		//GPIOC->ODR ^= 0x2000;
	};
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
