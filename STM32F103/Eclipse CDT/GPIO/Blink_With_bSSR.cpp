#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int i;
void delayms(int n);

int main(void)
{
	//Ahora habilito los relojes en los registros.
	RCC->APB2ENR |=0x10; //Se habilita GPIOC

	//Configuro los puertos
	GPIOC->CRH |= 0x100000; //Como es PC13 su configuración se encuentra en CRH y no en CRL.
							//Output PP, 10 MHz.

	while(1){
		GPIOC->BSRR = 0x20000000;
		delayms(250);
		GPIOC->BSRR = 0x2000;
		delayms(250);
	}
}

void delayms(int n){
	for(; n>0; n--){
		for(i =0; i<1595; i++);
	}
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
