/*
===============================================================================
 Name        : Eje10_Segura_Atencio.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "LPC17xx.h"
void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void confTimer(void); //Prototipo de la función de conf. de interrupciones externas
uint32_t aux = 0;
uint32_t aux2 = 0;
uint32_t shooter =0;
int main(void) {
SystemInit();
confGPIO();
confTimer();
while(1){}
return 0;
}
void confGPIO(void){
LPC_GPIO0->FIODIR |= (1<<22);
return;
}
void confTimer(void){
LPC_SC->PCONP |= (1 << 1); //Por defecto timer 0 y 1 estan siempre prendidos
LPC_SC->PCLKSEL0 |= (1<<2); // configuración del clock de periférico
LPC_PINCON->PINSEL3 |= (3<<20);
LPC_PINCON->PINSEL3 |= (3<<22);
LPC_PINCON->P
LPC_SC->PCLKSEL0 |= (3<<2);
LPC_TIM0->PR = 12499;
LPC_TIM0->CCR = 6;
LPC_TIM0->CCR |= (3<<4);
LPC_TIM0->TCR = 3;
LPC_TIM0->TCR &= ~(1<<1);
LPC_TIM0->IR |= 0xFF;
NVIC_EnableIRQ(TIMER0_IRQn);
return;
}
void TIMER0_IRQHandler(void) //ISR del timer0
{
static uint32_t i = 0;
 //Variable auxiliar para observar el valor del registro de captura
if (LPC_TIM0->IR & (1<<4)){
LPC_GPIO0->FIOSET = (1<<22);
aux = LPC_TIM0->CR0;
LPC_TIM0->IR|=(1<<4); //Limpia bandera de interrupción
}
else if (LPC_TIM0->IR & (1<<5)){
LPC_GPIO0->FIOCLR = (1<<22);
aux2 = LPC_TIM0->CR1;
shooter= aux2 - aux;
LPC_TIM0->IR|=(1<<5); //Limpia bandera de interrupción
}

return;
}
