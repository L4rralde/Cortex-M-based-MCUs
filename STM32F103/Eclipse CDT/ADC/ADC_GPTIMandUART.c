#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void UART1_Init(int bps);
char UART1_write(char dig);
char UART1_read(void);

//This program enables internal temperature sensor and sets ADC to read it
//periodically with TIM2 triggering it.

//TIM2 generates a PWM signal in its second channel (because first is not available for ADC)
//every rising_edge (or falling ?) ADC1 is triggered and starts a conversion.
//ADC only reads its CH16, which is connected to its internal temperature sensor.
//It is mandatory to enable this sensor in order to read its voltage signal.

//This sensor is not suitable for well measurements of absolute temperatures.
//Reference manual recommends to use for detecting temperature variations.
int main(){
	//Configuration of TIM2
	RCC->APB1ENR |= 1;
	TIM2->PSC = 7200-1;
	TIM2->ARR = 10000-1;
	TIM2->CNT = 0;
	TIM2->CCMR1 |= 0x6800; 	//CC2 channel of TIM2 is configured as PWM (mode 1)
							//output with preload register CCR1 enabled.

	TIM2->CCER |= 0x10; 	//CC2 output enabled.
	TIM2->CCR2 |= 50-1; 	//Value to be compared in order to generate PWM signal on CC2.
	TIM2->CR1 |= 1; 	//TIM2 enabled.

	//So, this TIMER has been configured as PWM output at its second channel with
	//a frequency of 1 Hz and a duty cycle of 50/10000. This is kind of a pulse (?).

	//Configuration of  ADC (and temperature sensor)
	RCC->APB2ENR |= 1<<9;	//Enables ADC1 clock
	ADC1->CR2 &= ~1;
	ADC1->SQR3 = 16; 	//First (only) reads ch16.
	RCC->CFGR |= 0x8000;	//ADC prescaler = 6;
			//ADC_f = APB2_f/6 = 72Mhz/6 = 12 MHz.
			//Temperature sensor is connected to CH16 of ADC1.
			//It's recommendable  to use a sampling time of 17.1 us.
			//It should take 205.2 cycles to get this sampling time. The nearest is the biggest (239.5).
	ADC1->SMPR1 = 0x001C0000; 	//239.5 cycles.
	ADC1->CR2 |= 1<<20; 	//Enables external trigger conversion for regular groups.
	ADC1->CR2 |= 3<<17; 	//External trigger: TIM2 CH2.
	//Falling or rising edge?
	ADC1->CR2 |= 1<<23 | 1; 	//Enables temperature sensor and its vref. Also, enables ADC
								//Manual recommends to enable both ADC and temp sensor at the same time.

	UART1_Init(115200);

	float voltage, temp;

	while(1){
		while(!(ADC1->SR & 2));	//Wait until an EOC (end of conversion)
		voltage = (float) ADC1->DR/4095 * 3.3;
		temp = (1.34-voltage)/0.004+25; 	//I use the minimum values because
											//they work better than the typical ones.
		temp /= 10;
		for(int i=0; i<4; ++i){
			UART1_write((int)temp+48);
			temp -= (int)temp;
			temp *=10;
		}
		UART1_write('\r');
		UART1_write('\n');

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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
