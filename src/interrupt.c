#include <stdint.h>
#include <math.h>
#include "motion.h"

int32_t Dx, Dy;
//dt = numTicks; //numTicks
// dx = x1-x0;
// dy = y1-y0;
/*dt2 = numTicks << 1; //These are used multiplied by two every iteration of the algorithm, so precompute them here to save time.
dx2 = dx << 1;
dy2 = dy << 1;
Dx = 2*dx-numTicks; //X decision variable
Dy = 2*dy-numTicks*/ //Y decision variable

typedef struct line {
  //Contains dt2, dx2, dy2, direction template
    int32_t dx2;
    int32_t dy2;
    int32_t dt2;
    int32_t numTicks;
    uint8_t direction; //Bit 0: x dir, Bit 1: y dir
};

line curLine; //Used as the current line

#define LINE_QUEUE_MAX 10
line lineQueue[LINE_QUEUE_MAX];
uint32_t lineQueueReadPos;
uint32_t lineQueueWritePos;

void writeLine(int32_t x_init, int32_t y_init, int32_t x_final, int32_t y_final, int32_t feedrate) //Units are mm and mm/s. x_init and y_init will be current position
{
    line temp;
    temp.dy2 = (y_final-y_init) << 1;
    temp.direction = 0x00;
    if (x_final >= x_init)
    {
        temp.direction |= 0x01;
        temp.dx2 = (x_final-x_init) << 1;
    }
    else
    {
        temp.dx2 = (x_init-x_final) << 1;
    }
    if (y_final >= y_init)
    {
        temp.direction |= 0x02;
        temp.dy2 = (y_final-y_init) << 1;
    }
    else
    {
        temp.dy2 = (y_init-y_final) << 1;
    }

    //Calculate total number of steps
    int32_t len = sqrt((temp.dx2 << 1)*(temp.dx2 << 1)+(temp.dy2 << 1)*(temp.dy2 << 1));
    float vel = feedrate / 60; //Convert from mm/min to mm/sec
    float time = len/vel;
    temp.numTicks = time*ISR_RATE;
    temp.dt2 = numTicks << 1;
}

void updateLine()
{
    if (!curLine.numTicks)
        return; //We wait for the current line to finish

    disableSteppers(); //Pause steppers until we are finished readying the next line

    lineQueueReadPos++;
    if (lineQueueReadPos >= LINE_QUEUE_MAX)
    {
        lineQueueReadPos = 0;
    }

    static line temp = lineQueue[lineQueueReadPos];
    curLine.dt2 = temp.dt2; //These have been pre-multiplied by 2 to save computation later
    curLine.dx2 = temp.dx2;
    curLine.dy2 = temp.dy2;
    curLine.numTicks = temp.numTicks >> 1; //How many interrupt ticks this line will take to draw
    Dx = dx2-numTicks; //X decision variable
    Dy = dy2-numTicks; //Y decision variable

    setDirection(temp.direction);
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
