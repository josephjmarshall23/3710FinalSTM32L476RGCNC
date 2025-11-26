#include <stdint.h>
#include "stm32l476xx.h"
#include "systick.h"
#include "stepper.h"

void SysTick_Initialize(uint32_t ticks) //Stolen straight from Dr. Zhu's slides
{
		SysTick->CTRL = 0; //Disable the thing
		SysTick->LOAD = ticks-1; //Set reload register
		NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1); //Set interrupt priority to least urgent
		SysTick->VAL = 0; //Reset counter to 0
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;//Set to processor clock source
		SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //Enable interrupt
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //Enable the thing
}

//This interrupt routine is now defined in interrupt.c, with the rest of the line drawing code.
//void SysTick_Handler(void)
//{
//
//}
