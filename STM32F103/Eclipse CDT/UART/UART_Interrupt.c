#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void UART1_Init(int bps);
char UART1_write(char dig);
char UART1_echo(void);

int main(){
	__disable_irq();

	//Configrue PC13 for led blinking
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH |= 0x100000;

	UART1_Init(115200);
	USART1->CR1 |= 1<<5; 	// A USART interrupt is generated whenever
							//ORE=1 or RXNE=1 in the USART_SR register.
	NVIC_EnableIRQ(USART1_IRQn);	//Enable USART1 interrupt on NVIC

	__enable_irq();
	while(1){
		//UART1_write('H');
		//UART1_write('o');
		//UART1_write('l');
		//UART1_write('a');
		//UART1_write('\r');
		//UART1_write('\n');
		//UART1_echo();
		GPIOC->ODR ^= 0x2000;
		for(int i=0; i<1000000; i++);
	}
	return 0;
}

void UART1_Init(int bps){
	RCC->APB2ENR |= 1<<2;	//PORTA's clock enabled
	GPIOA->CRH &= ~0xFF0; 	//Cleared config of PA9 and PA10.
	GPIOA->CRH |= 0x90;		//PA9 as AFPP output
	GPIOA->CRH |= 0x400; 	//PA10 as floating input

	RCC->APB2ENR |= 1<<14; //USART1's clock enabled
	USART1->BRR = 72000000/bps;
	USART1->CR1 |= 0xC;    //USART1 RX and TX enabled

	USART1->CR1 |= 1<<13; 	//USART1 enbled.
}

char UART1_write(char dig){
	while(!(USART1->SR & 0x80));
	USART1->DR = dig;
	return dig;
}

char UART1_read(void){
	while(!(USART1->SR & 0x20));
	return USART1->DR;
}

char UART1_echo(void){
	return(UART1_write(UART1_read()));
}

void USART1_IRQHandler(void){
	if(USART1->SR & 0x20){
		UART1_echo();
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
