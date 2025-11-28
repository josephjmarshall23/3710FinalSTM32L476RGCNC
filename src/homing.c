#include "homing.h"
#include "motion.h"
#include "main.h"
#include <stdint.h>

void homeX()
{
	uint32_t endstop = 0;
	int i;
	setDirection(0); //Set to negative X direction (towards X=0)
	while (!endstop)
	{
		endstop = GPIOC->IDR & X_LIMIT_BITPOS_GPIOC; //When triggered, will be 64 or 128 or some nonzero value
		stepX();
		for (i = 0; i < X_HOMING_DELAY; i++); //Delay between steps
	}
}

void homeY()
{
	uint32_t endstop = 1; //Must start high!
	int i;
	setDirection(2); //Set to positive Y direction (towards Y=Y_MAX_POS)
	while (endstop) //Endstop is different for Y- we move until the paper is NOT detected, then stop
	{
		endstop = GPIOB->IDR & Y_LIMIT_BITPOS_GPIOB; //When triggered, will be 64 or 128 or some nonzero value
		stepY();
		for (i = 0; i < Y_HOMING_DELAY; i++); //Delay between steps
	}
}
