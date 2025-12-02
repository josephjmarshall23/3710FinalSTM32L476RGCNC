#pragma once
#include <stdint.h>
#include <math.h>
#include "pins.h"
#include "stm32l476xx.h"

static inline void stepX() //Make sure ODR is marked volatile (otherwise may get optimized away)
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOA->ODR |= X_STEP_BITPOS_GPIOA; //Pulse high
    GPIOA->ODR &= (~X_STEP_BITPOS_GPIOA); //Pulse low
}

static inline void stepY() //Make sure ODR is marked volatile
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOB->ODR |= Y_STEP_BITPOS_GPIOB; //Pulse high
    GPIOB->ODR &= (~Y_STEP_BITPOS_GPIOB); //Pulse low
}

static inline void stepZ() //Make sure ODR is marked volatile
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOB->ODR |= Z_STEP_BITPOS_GPIOB; //Pulse high
    GPIOB->ODR &= (~Z_STEP_BITPOS_GPIOB); //Pulse low
}

static inline void disableSteppers() //Enable pin is active low
{
	GPIOA->ODR &= (~ENABLE_BITPOS_GPIOA);
}

static inline void enableSteppers() //Enable pin is active low
{
	GPIOA->ODR |= (ENABLE_BITPOS_GPIOA);
}

static inline void setDirection(uint8_t dir) //Bit 0: x dir, Bit 1: y dir
{
	uint32_t bsrr = 0x00000000;
	if (dir & 0x01) bsrr |= X_DIR_BITPOS_GPIOB;
	if (dir & 0x02) bsrr |= Y_DIR_BITPOS_GPIOB;
	if (!(dir & 0x01)) bsrr |= X_DIR_BITPOS_GPIOB << 16;
	if (!(dir & 0x02)) bsrr |= Y_DIR_BITPOS_GPIOB << 16;
	GPIOB->BSRR = bsrr; //Not sure why we didn't use this more. Top 16 bits reset a bit to 0, bottom 16 set a bit to 1.
}

static inline void penLift()
{
    GPIOA->ODR |= Z_DIR_BITPOS_GPIOA; //Set Z dir pin high
    for (int i = 0; i < 250; i++)
    {
    	stepZ();
        for (int j = 0; j < 83; j++); //Delay
    }
}

static inline void penUnlift()
{
    GPIOA->ODR &= (~Z_DIR_BITPOS_GPIOA); //Set Z dir pin low
    for (int i = 0; i < 250; i++)
    {
    	stepZ();
        for (int j = 0; j < 83; j++); //Delay
    }
}
