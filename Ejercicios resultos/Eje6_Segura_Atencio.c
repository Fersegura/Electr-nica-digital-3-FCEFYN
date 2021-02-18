/*
===============================================================================
 Name        : Eje6_Segura_Atencio.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
void iniciarPuertos(void);
void iniciarSystick(uint32_t);

int main(void) {
	SystemInit();
	iniciarPuertos();
	SysTick_Config(1000000);

	while(1) {

    }
    return 0 ;
}
void iniciarPuertos(){
	LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIOMASK = ~(1<<22);
	return ;
	}
void SysTick_Handler(){
	static uint32_t conta=0,estado=0;
	SysTick->CTRL &= (~(1<<16));
	if(estado){

		if(conta ==50){
			conta=0;
			estado = !estado;
			LPC_GPIO0->FIOSET |= (1<<22);
		}
		conta++;
	}
	else{

		if(conta == 100){
			estado=!estado;
			conta=0;

			LPC_GPIO0->FIOCLR |= (1<<22);
		}
			conta ++;
	}

	return 0;
}


