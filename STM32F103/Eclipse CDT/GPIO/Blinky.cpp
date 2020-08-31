#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, ulññññññpop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int i;
void delayms(int n);

int main(int argc, char* argv[])
{
//Programar un blink en el led de usuario.
//Es el PC13
//PC13 está conectado al bus APB2. APB2 está conectado a AHB2.
//Planeo usar HSE (la tarjeta integra un cristal de 8 MHz).
//Clock Control Register RCC_CR
//HCLK, AHB tienen la misma frecuencia, HCLK es el reloj del núcleo cortex.

//Para configurar un GPIO se usa GPIO_CRL y GPIO_CRH
	/**
	RCC->CR = 0x01030000;
	RCC->CR |= 0x02000000;
	RCC->CFGR = 0x003F0402; //SYSCLK; AHB sin pre-escalador; APB1=HCLK/2;
							//APB2=HCLK; ADC=APB2; PLL_IN=HSE/2; PLL_MULT=16;
							//Por lo tanto, sysclk=64, ahb=64, apb1=64, apb2=36
	**/ //Por el momento los dejaré por default.

	//Ahora habilito los relojes en los registros.
	RCC->APB2ENR |=0x10; //Se habilita GPIOC

	//Configuro los puertos
	GPIOC->CRH |= 0x100000; //Como es PC13 su configuración se encuentra en CRH y no en CRL.
							//Output PP, 10 MHz.

	while(1){
		GPIOC->ODR &= ~0x2000;
		delayms(1000);
		GPIOC->ODR |=0x2000;
		delayms(1000);
	}
}
void delayms(int n){
	for(; n>0; n--) {
		for(i =0; i<1597; i++);
	}
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
