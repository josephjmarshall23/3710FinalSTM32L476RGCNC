#include <stdint.h>
#include <math.h>
#include "pins.h"

inline void stepX() //Make sure ODR is marked volatile (otherwise may get optimized away)
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOA->ODR |= X_STEP_BITPOS_GPIOA; //Pulse high
    GPIOA->ODR &= (0xFFFFFFFF ^ X_STEP_BITPOS_GPIOA); //Pulse low
}

inline void stepY() //Make sure ODR is marked volatile
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOB->ODR |= Y_STEP_BITPOS_GPIOB; //Pulse high
    GPIOB->ODR &= (0xFFFFFFFF ^ Y_STEP_BITPOS_GPIOB); //Pulse low
}

inline void stepZ() //Make sure ODR is marked volatile
{
    //X,Y,Z step pins are PA10, PB3, PB5
    GPIOB->ODR |= Z_STEP_BITPOS_GPIOB; //Pulse high
    GPIOB->ODR &= (0xFFFFFFFF ^ Z_STEP_BITPOS_GPIOB); //Pulse low
}

inline void disableSteppers()
{
    GPIOA->ODR &= (0xFFFFFFFF ^ ENABLE_BITPOS_GPIOA);
}

inline void enableSteppers()
{
    GPIOA->ODR |= ENABLE_BITPOS_GPIOA;
}

inline void setDirection(uint8_t dir) //Bit 0: x dir, Bit 1: y dir
{
    GPIOB->ODR &= (0xFFFFFFFF ^ ((dir & 0x01) * X_DIR_BITPOS_GPIOB));
    GPIOB->ODR &= (0xFFFFFFFF ^ (((dir >> 1) & 0x01) * Y_DIR_BITPOS_GPIOB));
}

inline void penLift()
{
    GPIOA->ODR |= Z_DIR_BITPOS_GPIOA; //Z Dir pin is PA8
    for (int i = 0; i < 400; i++)
    {
        GPIOB->ODR |= Z_STEP_BITPOS_GPIOB;
        GPIOB->ODR &= (~Z_STEP_BITPOS_GPIOB);
        for (int j = 0; j < 100; j++);
    }
}

inline void penUnlift()
{
    GPIOA->ODR &= (~Z_DIR_BITPOS_GPIOA); //Z Dir pin is PA8
    for (int i = 0; i < 400; i++)
    {
        GPIOB->ODR |= Z_STEP_BITPOS_GPIOB;
        GPIOB->ODR &= (~Z_STEP_BITPOS_GPIOB);
        for (int j = 0; j < 100; j++);
    }
}
