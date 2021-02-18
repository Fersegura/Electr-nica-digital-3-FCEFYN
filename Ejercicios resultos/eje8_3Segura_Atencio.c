/*
===============================================================================
 Name        : eje8_3Segura_Atencio.c
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

void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void confTimer(void); //Prototipo de la función de conf. de interrupciones externas
void confEINT(void);
int main(void) {
	SystemInit();
	confGPIO();
	confTimer();
	confEINT();
	while(1){}
	return 0;
}
void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<22);

	return;
}
void confTimer(void){
	LPC_SC->PCONP |= (1 << 2); //Por defecto timer 0 y 1 estan siempre prendidos
	LPC_SC->PCLKSEL0 |= (1<<4); // configuración del clock de periférico
	LPC_SC->PCLKSEL0 &= ~(1<<5);
	//LPC_PINCON->PINSEL3 |= (3<<24);
	//LPC_TIM1->EMR|=(3<<4);
	LPC_TIM1->MR0 = 400000;
	//LPC_TIM1->MR1 = 100000-1;
	LPC_TIM1->PR = 1;
	LPC_TIM1->MCR |= (3<<0);
	LPC_TIM1->TCR = 3;
	LPC_TIM1->TCR &= ~(1<<1);
	NVIC_EnableIRQ(TIMER1_IRQn);
	return;
}
void confEINT(void){
	LPC_PINCON->PINSEL4 |= (1<<22); //pongo en modo EINT1 el P2.11
	LPC_SC->EXTPOLAR &= ~(1<<1); // defino que la interrupcion salte en flancos descendentes
	LPC_SC->EXTMODE |= (1<<1); //
	NVIC_EnableIRQ(EINT1_IRQn);
}

void EINT1_IRQHandler(void){
	LPC_TIM1->PR *=2;
	    	if(LPC_TIM1->PR > 1024)
	    	{
	    		LPC_TIM1->PR = 1;
	    		   	}
	LPC_SC->EXTINT |= (1<<1) ; //limpio la bandera de la interrupción externa
}

void TIMER1_IRQHandler(void) //ISR del timer0
{
    uint8_t flags = LPC_TIM1->IR;
	if((LPC_TIM1->IR)&(1<<0))
    {
       LPC_TIM1->IR |= (1<<0);       /* Clear the Interrupt Bit */
       LPC_GPIO0->FIOPIN = (flags<<22);
    }
	flags = LPC_TIM1->IR;

}
