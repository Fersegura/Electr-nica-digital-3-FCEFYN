/*
===============================================================================
 Name        : Ejercicio 1
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cr_section_macros.h>

#define FALSE 0
#define MAX_VALUE 10000000




// TODO: insert other include files here

// TODO: insert other definitions and
void retardo(int tiempo);
void retardo_secuencial(void);



int main(void) {
	SystemInit();
	uint32_t verClk;
	verClk = SystemCoreClock;
	LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIOMASK = ~(1<<22);

	while(1){

		LPC_GPIO0->FIOSET |= (1<<22);
		retardo_secuencial();
		LPC_GPIO0->FIOCLR |= (1<<22);
		retardo_secuencial();


	}
	 return 0 ;
}



void retardo (int tiempo){
	uint32_t counter;
	uint32_t *counter_address = &counter;

	printf("La direccion es: %x",counter_address);
	for(counter = 0; counter < tiempo; counter ++ ){}
	return;
}


void retardo_secuencial (void){

	static uint32_t UP_FADE = FALSE;
	static uint32_t counter=1000000;

	if(!UP_FADE){
		counter+= 500000;
		if (counter== MAX_VALUE){
			UP_FADE = ~UP_FADE;
		}
	}
	else {
		counter -=500000;
		if (counter == 0){
			UP_FADE = ~UP_FADE;
		}
	}

	for (int i =0; i<counter;i++){}

	return;


}
