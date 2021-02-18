/*
===============================================================================
 Name        : Eje11_Segura_Atencio.c
 Author      : Fernando Segura Atencio
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
#define DISP0 7
#define DISP1 8
uint8_t ESTADO =0;
uint8_t Cont1 =0, Cont2=0; //almacenan el valor de cada dígito
uint32_t tiempoSystick = 50;
void Actualizar();
void ConfGPIO();
void ConfSystick();
void SetValor(uint8_t);
void ConfTimer();

int main(void) {
	SystemInit();
	ConfSystick(tiempoSystick);
	ConfGPIO();
	ConfTimer();
	while(1);
    return 0 ;
}

void TIMER0_IRQHandler(void){
	if(Cont1<9)
	{
		Cont1++;
	}
	else{
		Cont1= 0;
		if(Cont2<5){
			Cont2++;
			}
		else{
			Cont2=0;
		}

	}
	LPC_TIM0->IR |= (1<<0);
}

void ConfTimer(){
	LPC_SC->PCONP |= (1<<1);
	LPC_SC->PCLKSEL0 |= (3<<2);
	LPC_TIM0->PR =12499;
	LPC_TIM0->MCR =0;
	LPC_TIM0->MCR |= (3<<0);
	LPC_TIM0->MR0 = 1000;
	LPC_TIM0->TCR = 3;
	LPC_TIM0->TCR &= ~(1<<1);
	NVIC_EnableIRQ(TIMER0_IRQn);
	NVIC_SetPriority(TIMER0_IRQn, (1<<3)-2);
	return;

}

void ConfGPIO(){
	LPC_GPIO0->FIODIR |= 0x1FF;
	LPC_GPIO2->FIODIR &= ~(3<<0);
	LPC_PINCON->PINMODE4 |= 0xF;
	LPC_GPIOINT->IO2IntEnR |= (3<<0);
	NVIC_EnableIRQ(EINT3_IRQn);

	return;
}

void EINT3_IRQHandler(){
	if(LPC_GPIOINT->IO2IntStatR & 1){
		LPC_GPIOINT->IO2IntClr |= (1<<0);
		if(tiempoSystick >5){
			tiempoSystick -= 5;
		}
	}
	if(LPC_GPIOINT->IO2IntStatR & (1<<1)){
			LPC_GPIOINT->IO2IntClr |= (1<<1);
			if(tiempoSystick <95){
				tiempoSystick += 5;
			}
		}
	ConfSystick(tiempoSystick);
}

void SysTick_Handler(){
	ESTADO = !ESTADO;
	Actualizar();
}


void ConfSystick(uint32_t tiempo){
	if (tiempo > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */

	  SysTick->LOAD  = (tiempo*100000 & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
	  NVIC_SetPriority (SysTick_IRQn, (1<<3)-1);  /* set Priority for Cortex-M0 System Interrupts */
	  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	  SysTick->CTRL  = (7<<0);                    /* Enable SysTick IRQ and SysTick Timer */
	  return (0);

}

void Actualizar(){
	if(ESTADO){
		SetValor(Cont1);
		LPC_GPIO0->FIOSET |=(1<<DISP0);
		LPC_GPIO0->FIOCLR |=(1<<DISP1);

		}
	else{
		SetValor(Cont2);
		LPC_GPIO0->FIOSET |=(1<<DISP1);
		LPC_GPIO0->FIOCLR |=(1<<DISP0);

	}

}

void SetValor(uint8_t valor){
	switch (valor) {
					case 0:
						LPC_GPIO0->FIOPIN =0x3F;
						break;
					case 1:
						LPC_GPIO0->FIOPIN = 0x6;
						break;
					case 2:
						LPC_GPIO0->FIOPIN = 0x5B;
						break;
					case 3:
						LPC_GPIO0->FIOPIN = 0x4F;
						break;
					case 4:
						LPC_GPIO0->FIOPIN = 0x66;
						break;
					case 5:
						LPC_GPIO0->FIOPIN = 0x6D;
						break;
					case 6:
						LPC_GPIO0->FIOPIN = 0x7D;
						break;
					case 7:
						LPC_GPIO0->FIOPIN = 0x7;
						break;
					case 8:
						LPC_GPIO0->FIOPIN = 0x7F;
						break;
					case 9:
						LPC_GPIO0->FIOPIN = 0x67;
						break;
									}
}
