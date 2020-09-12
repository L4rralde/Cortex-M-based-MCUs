#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

const int SYSCLK = 72000000;

struct UART_periph{
	int baudRate;
	int *peripheralClk;
	//char enClk;
	//int GPIOPort;
	//char txBit;
	//char rxBit;
	//int GPIOClk;
	//char GPIOEn;
};

//UART_periph UART1 = {0, *(RCC->APB1ENR)};
int main(void){

	while(true){

	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
