/*
===============================================================================
 Name        : Eje9_Segura_Atencio
 Author      : Fernando Segura
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "LPC17xx.h"

/*
 * 9.1
 */

void confGPIO(void);  // Prototipo de la funcion de conf. de puertos

void confTimer(void); //Prototipo de la función de conf. de timer

void confEINT3(void); //Prototipo de la funcion de conf de interrupciones
					  //externas.



uint8_t estado=0; //Variable global que indica el tipo de secuencia
				  //que provee el programa. Global porque se modifica en
				  //2 funciones distintas




int main(void) {

	SystemInit();	//inicializacion del sistema

	confGPIO();		//Configuracion de los puertos



	confEINT3();	//Configuracion de las interrupciones externas

	confTimer();   //configuracion del timer
	while (1) {		//bucle infinito

	}

	return 0;
}

void confGPIO(void) {
	LPC_GPIO0->FIODIR |= (0x0000000F << 0);			//Seleccionamos p0.0, p0.1
													//p0.2 y p0.3 como salidas
	LPC_GPIO0->FIOMASK &= ~((0x0000000F << 0));

	return;
}

void confEINT3(void) {
	LPC_PINCON->PINSEL4 |=  (1 << 26);	//Seleccionamos que el pin

	LPC_PINCON->PINSEL4 &= ~(1<<27);

	LPC_SC->EXTMODE |= (1 << 3);					//p2.13 como fuente de int

	LPC_SC->EXTPOLAR &= (0 << 3);					//externa y lo hacemos fun-

	NVIC_SetPriority(EINT3_IRQn, 15);				//cionar por flanco desc.

	NVIC_EnableIRQ(EINT3_IRQn);						//prioridad 15

	return;
}

void confTimer(void) {

	LPC_SC->PCONP |= (1 << 1);  	//Aseguramos que este encendido el tmr0

	LPC_SC->PCLKSEL0 |= (1 << 2);// Frecuencia del periferico 100 MHz

	LPC_SC->PCLKSEL0 &= ~(1<<3);
	//	LPC_PINCON->PINSEL3 |= (3 << 24);

	//	LPC_TIM0->EMR |= (3 << 4);

	LPC_TIM0->PR = 999;				//Prescaler a 0

	LPC_TIM0->MR0 = 499999;		//Match0 con el valor para 5ms

	LPC_TIM0->MR1 = 999999;		//Match1 con el valor para 10ms

	LPC_TIM0->MR2 = 1499999;		//Match2 con el valor para 15ms

	LPC_TIM0->MR3 = 1999999;		//Match3 con el valor para 20ms

	LPC_TIM0->MCR = 0x649 ; 	//Todos los match generan irq pero solo
									//el ultimo resetea el timer
	LPC_TIM0->TCR = 3;				//Iniciamos el timer y lo reseteamos

	LPC_TIM0->TCR &= ~(1 << 1);

	LPC_TIM0->IR |= 0x3F; 			//limpiamos cualquier bandera del timer

	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}

void TIMER0_IRQHandler(void) //ISR del timer0
{
	 uint8_t flags ;

	flags = (LPC_TIM0->IR &0xF);

	if (estado == 0) {
		//secuencia Nº1
		switch (flags) {
		case 1:
			LPC_GPIO0->FIOSET = (1 << 0);
			LPC_GPIO0->FIOCLR = (1 << 3);
			LPC_TIM0->IR |= 1;
			break;
		case 2:
			LPC_GPIO0->FIOSET = (1 << 1);
			LPC_GPIO0->FIOCLR = (1 << 0);
			LPC_TIM0->IR |= 2;
			break;
		case 4:
			LPC_GPIO0->FIOSET = (1 << 2);
			LPC_GPIO0->FIOCLR = (1 << 1);
			break;
			LPC_TIM0->IR |= 4;
		case 8:
			LPC_GPIO0->FIOSET = (1 << 3);
			LPC_GPIO0->FIOCLR = (1 << 2);
			LPC_TIM0->IR |= 8;
			break;
		default:
			LPC_TIM0->IR = 0xF;
			break;
		}
	} else {

		switch (flags) {
		case 1:
			LPC_GPIO0->FIOSET = (1 << 1);
			LPC_GPIO0->FIOCLR = (1 << 3);
			LPC_TIM0->IR |= 1;
			break;
		case 2:
			LPC_GPIO0->FIOSET = (1 << 2);
			LPC_GPIO0->FIOCLR = (1 << 0);
			LPC_TIM0->IR |= 2;
			break;
		case 4:
			LPC_GPIO0->FIOSET = (1 << 3);
			LPC_GPIO0->FIOCLR = (1 << 1);
			LPC_TIM0->IR |= 4;
			break;
		case 8:
			LPC_GPIO0->FIOSET = (1 << 0);
			LPC_GPIO0->FIOCLR = (1 << 2);
			LPC_TIM0->IR |= 8;
			break;
		default:
			break;
		}
	}
	return;
}

void EINT3_IRQHandler(void) {
	static uint8_t i=0;
	if(i==0){
		estado = 0;
		LPC_TIM0->TCR = 3;
		LPC_TIM0->MR0 = 0x0007A120;
		LPC_TIM0->MR1 = 0X000F4240;
		LPC_TIM0->MR2 = 0X0016E360;
		LPC_TIM0->MR3 = 0X001E8480;
		LPC_TIM0->TCR &= ~(1 << 1);
		i=1;
	}else{
		estado = 1;
		LPC_TIM0->TCR = 3;
		LPC_TIM0->MR0 = 0X0003D090;
		LPC_TIM0->MR1 = 0X0007A120;
		LPC_TIM0->MR2 = 0X000B71B0;
		LPC_TIM0->MR3 = 0X000F4240;
		LPC_TIM0->TCR &= ~(1 << 1);
		i=0;
	}
	LPC_SC->EXTINT = (1 << 3);
	return;
}
