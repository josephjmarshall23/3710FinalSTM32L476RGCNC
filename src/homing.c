#include "homing.h"
#include "motion.h"
#include "main.h"
#include <stdint.h>

void homeX()
{
	uint32_t endstop = 0;
	int i;
	int count = 0;
	const int threshold_x = 4; //X endstop doesn't have as much noise as Y
	setDirection(0); //Set to negative X direction (towards X=0)
	while (count < threshold_x)
	{
		stepX();
		for (i = 0; i < X_HOMING_DELAY; i++); //Delay between steps
		endstop = GPIOC->IDR & X_LIMIT_BITPOS_GPIOC; //When triggered, will be 64 or 128 or some nonzero value
		if (!endstop) count = 0;
		if (endstop) count++;
	}
}

void homeY()
{
	uint32_t endstop = 1; //Must start high!
	int i;
	int count = 0;
	const int threshold_y = 10; //Y endstop is very noisy
	setDirection(2); //Set to positive Y direction (towards Y=Y_MAX_POS)
	while (count < threshold_y) //Endstop is different for Y- we move until the paper is NOT detected, then stop
	{
		stepY();
		for (i = 0; i < Y_HOMING_DELAY; i++); //Delay between steps
		endstop = GPIOB->IDR & Y_LIMIT_BITPOS_GPIOB; //When triggered, will be 64 or 128 or some nonzero value
		if (!endstop) count++;
		if (endstop) count = 0;
	}
	//At this point, Y = 220 mm (out of 279, near the top of page)
	//Y min is about Y = 20 mm

}
