#pragma once
#include <stdint.h>
#include <math.h>
#include "pins.h"

inline void stepX(); //Make sure ODR is marked volatile (otherwise may get optimized away)

inline void stepY(); //Make sure ODR is marked volatile

inline void stepZ(); //Make sure ODR is marked volatile

inline void disableSteppers();

inline void enableSteppers();

inline void setDirection(uint8_t dir); //Bit 0: x dir, Bit 1: y dir

inline void penLift();

inline void penUnlift();
