#pragma once
#include <stdint.h>
#include "main.h"

#define PEN_LIFT 	0x04
#define PEN_DROP 	0x08
#define HOME_X 		0x10
#define HOME_Y		0x20
#define ENABLE_STP  0x40
#define DISABLE_STP 0x80

typedef struct {
    int32_t dx2; //Number of X steps times 2
    int32_t dy2; //Number of Y steps times 2
    int32_t dt2; //Number of interrupt ticks times 2
    int32_t numTicks; //Number of interrupt ticks (if 0, perform action instead of draw line)
    uint8_t direction; //Stores direction and actions. Actions performed if numTicks == 0.
    //Bit 0: x dir, Bit 1: y dir, Bit 2: pen lift, Bit 3: pen drop, Bit 4: X home, Bit 5: Y home, Bit 6: Enable, Bit 7: Disable
} line;

int pushToLineQueue(line in);
int popFromLineQueue(line *out);
void writeLine(float x_dist, float y_dist, float feedrate);
void writeAction(uint8_t action);
void updateLine();
//void SysTick_Handler(); //Doesn't need to be declared, but just for reference

extern uint8_t is_room_in_buffer;
extern uint32_t lineQueueCount;
static inline int getLineQueueSpace()
{
	return (LINE_QUEUE_MAX - lineQueueCount);
}
