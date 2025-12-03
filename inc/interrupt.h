#pragma once
#include <stdint.h>
#include "main.h"


typedef struct {
  //Contains dt2, dx2, dy2, direction template
    int32_t dx2;
    int32_t dy2;
    int32_t dt2;
    int32_t numTicks;
    uint8_t direction; //Bit 0: x dir, Bit 1: y dir
} line;

int pushToLineQueue(line in);
int popFromLineQueue(line *out);
void writeLine(float x_dist, float y_dist, float feedrate);
void updateLine();
//void SysTick_Handler(); //Doesn't need to be declared, but just for reference

extern uint8_t is_room_in_buffer;
extern uint32_t lineQueueCount;
static inline int getLineQueueSpace()
{
	return (LINE_QUEUE_MAX - lineQueueCount);
}
