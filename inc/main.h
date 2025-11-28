#pragma once

#define MAX_FEEDRATE 10000 //In mm/min
#define X_STEPS_PER_MM 80 //Exactly correct (theoretically, belt axis)
#define Y_STEPS_PER_MM 69 //Needs to be adjusted to correct value, but should be close (calculated 69.3)
#define X_STEPS_PER_MM_2 2*X_STEPS_PER_MM //These are just defined here to prevent computing them often in the hot loop.
#define Y_STEPS_PER_MM_2 2*Y_STEPS_PER_MM
#define X_HOMING_DELAY 20
#define Y_HOMING_DELAY 20
#define INTERRUPT_RATE 40000 //How often the SysTick interrupt is called, which updates the steppers
#define LINE_QUEUE_MAX 2000
#define ARC_LINE_LENGTH 1 //Arcs are divided into ~1mm length lines.
#define ARC_MAX_SEGMENTS 100
