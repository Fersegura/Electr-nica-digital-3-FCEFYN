/*
===============================================================================
 Name        : eje2_Segura_Atencio_2.c
 Author      : Fernando Segura Atencio
 Version     :	1
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

void inicial(void);
void retardo(uint32_t);
void avanzar(uint32_t);
int main(void) {
	SystemInit();
	uint32_t posicion,sentido,velocidad;
	posicion=0,
	sentido=0;
	velocidad=1000000;
	inicial();
	while(1){
		if(!(LPC_GPIO2->FIOPIN &= (1<<0)))
		{sentido =!sentido;}
		if(!(LPC_GPIO2->FIOPIN &= (1<<1)))
		{velocidad += 20000;}
		if(!(LPC_GPIO2->FIOPIN &= (1<<2)))
				{velocidad -= 20000;}
		if(sentido){
			if(posicion==0){
				posicion=7;
			}
			else{
				posicion--;
				}
		}
		else{
		    if(posicion==7){
			   posicion=0;
						}
			else{
			   posicion++;
							}
	}
		avanzar(posicion);
	    retardo(velocidad);
	}


    return 0 ;
}
void avanzar(uint32_t p){
	LPC_GPIO0->FIOCLR |= 0xFF;
	LPC_GPIO0->FIOSET |= (1<<p);
}
void retardo(uint32_t t){
	for(int i=0;i<t;i++){}
		return 0;
}


void inicial(){
	LPC_GPIO0->FIODIR |= 0xFF;

	LPC_GPIO2->FIODIR &= (~(1<<0 | 1<<1 | 1<<2));
	LPC_GPIO0->FIOMASK &= ~(0xFF);
	return 0;
	}
