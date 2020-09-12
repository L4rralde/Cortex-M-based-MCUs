//This code only works in C, not in C++
//Since now, I rather use C instead of C++

#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//This project works properly.

int main(void){

	//This time this is much different than its similar in STMF411
	//We first disable interruptions before configuring them.

	//Configure pins:
	//Input (external interrupt) as floating input
	//Output (led to toggle) as push-pull output

	//To configure external interruption
		//Enable AFIO clock

		//Hardware interrupt selection
			//To configure the 20 lines as interrupt sources, use the following procedure:
			//• Configure the mask bits of the 20 Interrupt lines (EXTI_IMR)
			//• Configure the Trigger Selection bits of the Interrupt lines (EXTI_RTSR and
			//EXTI_FTSR)
			//• Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
			//External Interrupt Controller (EXTI) so that an interrupt coming from one of the 20 lines
			//can be correctly acknowledged.

	//Configure NVIC (integrated in cortex, look ar cortex m3 manual)
	//Enable irq

	__disable_irq();

	//PB11 as external interruption
	RCC->APB2ENR |= 1<<3;
	//GPIOB->CRH |= 0x4000; //This line isn't necessary

	//For the output to toggle: PC13 as push-pull output
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	//I'm using PA0 for a led blinking in order to see
	//how a interruption actually interrupts main program.
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL |= 1;

	//To configure external configuration on PB11:

	EXTI->IMR |= 1<<11; 	//Unmask line 11 interruption
	EXTI->FTSR |= 1<<11; 	//Trigger on falling edge
	//EXTI->RTSR |= 1<<11; 	//trigger on rising edge

	RCC->APB2ENR |= 1;	//Enable AFIO clock
	AFIO->EXTICR[2] &= ~0xF000;
	AFIO->EXTICR[2] |= 0x1000; //External interruption line 11 on PORT B

	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();

	while(1){
		GPIOA->ODR ^= 0x1;
		for(int i=0; i<250000L; i++);
	}

	return 0;
}

//Here I defined a predefined function to handle the interruption.
//In the function, flag need to be disabled.

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & 1<<11) != 0){
		int rep=0;
		int i=0;
		for(; rep<1; rep++){
			GPIOC->ODR ^= 0x2000;
			for(i=0; i<1000000; i++);
		}
		EXTI->PR |= 0x1<<11;
	}
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
