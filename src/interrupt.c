#include <stdint.h>
#include <math.h>
#include "motion.h"
#include "main.h"
#include "interrupt.h"
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
    time = len*120/(feedrate); //time is in seconds now (converted feedrate from mm/min to mm/sec) (Also compensated for being off by 2x)
    temp.numTicks = time*INTERRUPT_RATE;
    temp.dt2 = temp.numTicks << 1; //Multiply by 2
    while (!getLineQueueSpace()); //Wait until ready
    pushToLineQueue(temp);
}

void updateLine() //Should be called repeatedly in main.c.
{
	is_room_in_buffer = (getLineQueueSpace() > ARC_MAX_SEGMENTS+1) ? 1 : 0;
    if (curLine.numTicks)
        return; //We wait for the current line to finish

    disableSteppers(); //Pause steppers until we are finished readying the next line

    static line tmp;
    if (!popFromLineQueue(&tmp)) return; //If nothing in queue, return

    curLine.dt2 = tmp.dt2; //These have been pre-multiplied by 2 to save computation later
    curLine.dx2 = tmp.dx2;
    curLine.dy2 = tmp.dy2;
    curLine.numTicks = tmp.numTicks >> 1; //How many interrupt ticks this line will take to draw
    Dx = tmp.dx2-tmp.numTicks; //X decision variable
    Dy = tmp.dy2-tmp.numTicks; //Y decision variable

    setDirection(tmp.direction);
    enableSteppers(); //Re-enable steppers
}

void SysTick_Handler()  //We replace dx and dy with dt (ticks), dx (x steps), dy (y steps). Completes one round of Bresenham's algorithm.
{
    if (!curLine.numTicks)
        return;

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
