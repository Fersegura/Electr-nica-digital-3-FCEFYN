/*
===============================================================================
 Name        : Eje7_Segura_Atencio.c
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
void SysTick_Handler(void);
void iniciarPuerto(void);
void confEINT0(void);
void EINT0_IRQHandler(void);
static int valorHigh=1;
int main(void) {
	SystemInit();
	iniciarPuerto();
	confEINT0();
	SysTick_Config(100000); //configuro para que sea 1 ms y se interrumpa.
    while(1) {

    }
    return 0 ;
}

void iniciarPuerto(){
	LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIOMASK |= (~((1<<22)));
	LPC_PINCON->PINSEL4 = (1<<20);
	return ;
}
void SysTick_Handler(){
	SysTick->CTRL &= (~(1<<16));
	static uint32_t conta=0;
	if(conta ==10){
		conta=0;
	}
	if (conta<valorHigh)
	{
		conta++;
		LPC_GPIO0->FIOSET |= (1<<22);
	}
	else{
		conta++;
		LPC_GPIO0->FIOCLR |= (1<<22);
	}
	return ;

}
void confEINT0(void){

	LPC_SC->EXTMODE |= (1<<0);
	LPC_SC->EXTPOLAR |=(1<<0);
	NVIC_EnableIRQ(EINT0_IRQn);
	return ;
}
void EINT0_IRQHandler(void){

		valorHigh ++;
		if(valorHigh==11)
		{
			valorHigh=1;
		}


	LPC_SC->EXTINT |= (1<<0) ;
	return ;
}
