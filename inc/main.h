#pragma once

#define MAX_FEEDRATE 1000 //In mm/s. The slower this is, the longer moves we can store in our limited memory.
#define MAX_STEPRATE (MAX_FEEDRATE/40)*3200 //In steps per second
#define X_STEPS_PER_MM 80
#define Y_STEPS_PER_MM 60 //Needs to be adjusted to correct value
#define INTERRUPT_RATE 40000 //How often the SysTick interrupt is called, which updates the steppers
