/*
===============================================================================
 Name        : ej13Maier.c
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


void confADC();
void confTimer();
uint16_t ADC0Value=0;
void confGPIO();

int main(void) {
	SystemInit();
	confTimer();
	confGPIO();
	confADC();
	while(1){}
	return 0;

}

void confADC(){
	LPC_SC->PCONP |= (1<<12); // Habilita el periferico.
	LPC_ADC->ADCR |= (1<<21); // El conversor es operacional.
	LPC_SC->PCLKSEL0 |= (3<<24); // CCLK/8 = 12,5 Mhz.
	LPC_ADC->ADCR &=~(255<<8); // Divide por 1 el clock que llega al ADC.
	LPC_ADC->ADCR &=~(1<<16); // No burst mode.
	LPC_ADC->ADCR |= (1<<26); //Empieza la conversion cuando ocurre el flanco de MATCH0.1.
	LPC_ADC->ADCR &= ~(3<<24); // 011->100 bit 24 y 25 en 0.
	LPC_ADC->ADCR |= (1<<27); // Matchea con flanco descendente.
	LPC_PINCON->PINSEL4 |=(1<<20); // Habilita EINT0.
	LPC_PINCON->PINMODE1 |=(1<<15);// Ni pull-up, ni pull-down.
	LPC_PINCON->PINSEL1 |=(1<<14);// Canal 0.0 del ADC.
	LPC_PINCON->PINSEL3 |= (3<<26);
	LPC_ADC->ADINTEN = 1; // Habilita interrupciones para el canal 0.
	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void ADC_IRQHandler(){
	ADC0Value=((LPC_ADC->ADDR0)>>4)&0xFFF;
	LPC_GPIO0->FIOPIN = ADC0Value;
	return;
}

void confGPIO(){
	LPC_GPIO0->FIODIR |= (0xFFF<<0);
	LPC_GPIO0->FIOMASK |= ~(0xFFF<<0);
}

void confTimer(){
	LPC_SC->PCONP |= (1<<1); // Habilita el timer 0 y 1.
	LPC_SC->PCLKSEL0|=(1<<2); // PCLK=cclk
	LPC_TIM0->EMR|=(3<<6); // Toggle
	LPC_TIM0->MR1=4999; // Interrumpe c/ 5 seg.
	LPC_TIM0->PR = 0;
	LPC_TIM0->MCR=(1<<4); // Reset TC cuando matchea.
	LPC_TIM0->TCR =3; // Reset TC y PC.
	LPC_TIM0->TCR &= ~(1<<1); // Deja de resetear y comienza a contar.
	return;
}
