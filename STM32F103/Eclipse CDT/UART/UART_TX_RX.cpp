#include "stm32f10x.h"
//#include <math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

char string[32];

void delayMs(int delay);
char USART1_write(char msg);
char USART1_read(void);
char fputc(char msg);
char* input(void);
void print(char*);

int main(void){
//For this project, I intend to echo messages received from the computer
//using USART1 module

//Step 1: Enable clocks
	RCC->APB2ENR |= 1<<14;
	RCC->APB2ENR |= 1<<2;

//Step 2: Set up terminals
	GPIOA->CRH |= 0x400; 	//PA10 (Rx) as input floating
	GPIOA->CRH |= 0x90;  	//PA9 (Tx) as AF push-pull output

//Step 3: Set up USART1 Module
	USART1->BRR = 0x271; 	//1152000 @72MHz
	USART1->CR1 |= 1<<2; 	//Rx enable
	USART1->CR1 |= 1<<3; 	//Tx enable
	USART1->CR1 |= 1<<13; 	//USART1 Module enable

//Others:

    while (1){
    	//data = gets();
    	//USART1_write(data[1]);
    	//data = gets();
    	//print(data);
    	print(input());
    	//print("Miguelito, como estas?\r");
    	//probe = strtofloat(data);
    	//if(probe>10) print("Si\r");
    }
}

void delayMs(int delay){
	int i = 0;
	for(; delay>0; delay--) for(i=0 ; i<1965; i++);
}

char USART1_write(char msg){
	while(!(USART1->SR & 0x80));
	USART1->DR = msg;
	return msg;
}

char USART1_read(void){
	while(!(USART1->SR & 0x20));
	return USART1->DR;
}

char * input(void){
	int pos = 0;
	bool stop = false;
	char aux = 0;
	//char string[32];
	while(!stop){
		aux = USART1_read();
		switch(aux){
			case 8:
				pos--;
				USART1_write(8);
				USART1_write(' ');
				USART1_write(8);
				break;
			case 13:
				USART1_write(10);
				USART1_write(13);
				string[pos] = aux;
				stop = true;
				break;
			default:
				USART1_write(aux);
				string[pos] = aux;
				pos++;
				break;
		}
		pos *= (pos>0);
	}
	stop = true;
	return string;
}

void print(char * a){
	int i=0;
	char aux = *a;
	while(aux != 13){
		USART1_write(aux);
		i++;
		aux = *(a + i);
	}
	USART1_write(10);
	USART1_write(13);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
