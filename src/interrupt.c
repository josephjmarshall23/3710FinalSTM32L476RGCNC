#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "motion.h"
#include "main.h"
#include "interrupt.h"
#include "homing.h"
#include "UART.h"

int32_t Dx, Dy;
//dt = numTicks; //numTicks
// dx = x1-x0;
// dy = y1-y0;
/*dt2 = numTicks << 1; //These are used multiplied by two every iteration of the algorithm, so precompute them here to save time.
dx2 = dx << 1;
dy2 = dy << 1;
Dx = 2*dx-numTicks; //X decision variable
Dy = 2*dy-numTicks*/ //Y decision variable

line curLine; //Used as the current line

line lineQueue[LINE_QUEUE_MAX];
uint32_t lineQueueReadPos;
uint32_t lineQueueWritePos;
uint32_t lineQueueCount;
uint8_t is_room_in_buffer; //0 if no room, 1 if at least 100 slots

int pushToLineQueue(line in)
{
	if (lineQueueCount == LINE_QUEUE_MAX) return 0; //No space to push

	lineQueue[lineQueueWritePos] = in;
	lineQueueWritePos++;
	if (lineQueueWritePos >= LINE_QUEUE_MAX)
		lineQueueWritePos = 0;
	lineQueueCount++;
	return 1;
}

int popFromLineQueue(line *out)
{
	if (lineQueueCount == 0) return 0;

	*out = lineQueue[lineQueueReadPos];
	lineQueueReadPos++;
	if (lineQueueReadPos >= LINE_QUEUE_MAX)
		lineQueueReadPos = 0;
	lineQueueCount--;
	return 1;
}

void writeLine(float x_dist, float y_dist, float feedrate) //Units are mm and mm/s. x_init and y_init will be current position
{
	float len, time; //Static so we don't make a new one every time
	static line temp;

    temp.direction = 0x00;
    if (x_dist >= 0.0f)
    {
        temp.direction |= 0x01;
        temp.dx2 = (int32_t) ((x_dist)*X_STEPS_PER_MM_2);
    }
    else
    {
        temp.dx2 = (int32_t) ((-x_dist)*X_STEPS_PER_MM_2);
    }
    if (y_dist >= 0.0f)
    {
        temp.direction |= 0x02;
        temp.dy2 = (int32_t) ((y_dist)*Y_STEPS_PER_MM_2);
    }
    else
    {
        temp.dy2 = (int32_t) ((-y_dist)*Y_STEPS_PER_MM_2);
    }

    //Calculate total number of steps
    len = sqrt((x_dist*x_dist)+(y_dist*y_dist));
    if (feedrate > MAX_FEEDRATE) //Make sure we don't go over the speed limit
    	feedrate = MAX_FEEDRATE;
    time = len*60/(feedrate); //time is in seconds now (converted feedrate from mm/min to mm/sec) (Also compensated for being off by 2x)
    temp.numTicks = time*INTERRUPT_RATE;
    temp.dt2 = temp.numTicks << 1; //Multiply by 2
    while (!getLineQueueSpace()) ;//USART_Write(USART2, (unsigned char*)"noLineQueueSpace\r\n", 18); //Wait until ready
    pushToLineQueue(temp);
}

void writeAction(uint8_t action)
{
	line temp;
	temp.numTicks = 0;
	temp.direction = action;
	while (!getLineQueueSpace());
	pushToLineQueue(temp);
}

void updateLine() //Should be called repeatedly in main.c.
{
	is_room_in_buffer = (getLineQueueSpace() > ARC_MAX_SEGMENTS+1) ? 1 : 0;
    if (curLine.numTicks > 0)
        return; //We wait for the current line to finish
//    USART_Write(USART2, (unsigned char*)"done with line\r\n", 16);
    //disableSteppers(); //Pause steppers until we are finished readying the next line

    static line tmp;
    if (!popFromLineQueue(&tmp)) return; //If nothing in queue, return

    //Check if this is not a line to be drawn but actions to be performed
    if (tmp.numTicks == 0)
    {
    	if ((tmp.direction & PEN_LIFT)) penLift(); //Order of first 4 is intentional- can do first 4 in this order with one line in queue
    	if ((tmp.direction & HOME_X)) homeX();
    	if ((tmp.direction & HOME_Y)) homeY();
    	if ((tmp.direction & PEN_DROP)) penUnlift();
    	if ((tmp.direction & ENABLE_STP)) enableSteppers();
    	if ((tmp.direction & DISABLE_STP)) disableSteppers();
    	return; //Actions only happen on a separate queue item from an actual drawn line
    }

    curLine.dt2 = tmp.dt2; //These have been pre-multiplied by 2 to save computation later
    curLine.dx2 = tmp.dx2;
    curLine.dy2 = tmp.dy2;
    curLine.numTicks = tmp.numTicks; //How many interrupt ticks this line will take to draw
    Dx = tmp.dx2-tmp.numTicks; //X decision variable (global)
    Dy = tmp.dy2-tmp.numTicks; //Y decision variable (global)
//    char str[40];
//    sprintf(str, "DX:%015ld DY:%015ld\r\n", Dx, Dy);
//	USART_Write(USART2, (unsigned char*)str, 39);

    setDirection(tmp.direction);
    enableSteppers(); //Re-enable steppers
}

void SysTick_Handler()  //We replace dx and dy with dt (ticks), dx (x steps), dy (y steps). Completes one round of Bresenham's algorithm.
{
    if (curLine.numTicks <= 0)
    {
        return;
    }

    if (Dx > 0)
    {
        stepX();
        Dx -= curLine.dt2;
    }
    Dx += curLine.dx2;
    if (Dy > 0)
    {
        stepY();
        Dy -= curLine.dt2;
    }
    Dy += curLine.dy2;
    curLine.numTicks--;
}
