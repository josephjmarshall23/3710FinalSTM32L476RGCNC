#include "StepBuffer.h"
#include "main.h"
#include <stdint.h>
#include <math.h>

//The step buffer is one large queue.
//Each entry is one byte, and holds info on which steppers to move one step at that time, as well as what direction to move them.
#define STEP_BUFFER_SIZE 64000 //Amount of bytes our step buffer takes.
#define X_DIR_BIT_POS 0x80
#define Y_DIR_BIT_POS 0x40
#define PEN_DIR_BIT_POS 0x20
#define X_STEP_BIT_POS 0x10
#define Y_STEP_BIT_POS 0x08
#define PEN_STEP_BIT_POS 0x04
uint8_t stepBuffer[STEP_BUFFER_SIZE]; //Bit 7,6,5: x,y,pen dir, bit 4,3,2: x,y,pen step, bit 1,0: reserved
uint16_t readLoc;
uint16_t writeLoc;

uint16_t getStepsAvailable()
{
    if (writeLoc <= readLoc)
    {
        return (readLoc-writeLoc);
    }
    else
    {
        return ((readLoc)+(64000-writeLoc));
    }
}

uint8_t popFromBuffer()
{
    uint8_t temp = stepBuffer[readLoc++];
    if (readLoc >= STEP_BUFFER_SIZE)
        readLoc -= STEP_BUFFER_SIZE;
    return temp;
}

void pushToBuffer(uint8_t data)
{
    stepBuffer[writeLoc++] = data;
    if (writeLoc >= STEP_BUFFER_SIZE)
        writeLoc -= STEP_BUFFER_SIZE;
    return;
}

//Writes steps to stepBuffer. Returns 0 if successful. Returns number of buffer entries required if not successful.
uint16_t lineToSteps(uint32_t xInitPos, uint32_t yInitPos, uint32_t xFinalPos, uint32_t yFinalPos, uint32_t feedrate)
{
    //Get distance to move and direction
    uint32_t xDist, yDist;
    uint8_t direction = 0x00; //This is like a template that will be copied for every step command
    if (xFinalPos >= xInitPos)
    {
        xDist = xFinalPos-xInitPos;
        direction |= X_DIR_BIT_POS;
    }
    else
    {
        xDist = xInitPos-xFinalPos;
    }
    if (yFinalPos >= yInitPos)
    {
        yDist = yFinalPos-yInitPos;
        direction |= Y_DIR_BIT_POS;
    }
    else
    {
        yDist = yInitPos-yFinalPos;
    }

    //Calculate x and y feedrates
    float xDistf = (float) xDist;
    float yDistf = (float) yDist;
    float totalDistf = sqrt(xDistf*xDistf+yDistf*yDistf);
    float xFeedrate = (xDistf/totalDistf)*((float) feedrate);
    float yFeedrate = (yDistf/totalDistf)*((float) feedrate);

    //Convert to step rate
    xFeedrate *= X_STEPS_PER_MM;
    yFeedrate *= Y_STEPS_PER_MM;

    //Calculate how many buffer entries this line will use
    xDistf *= (MAX_STEPRATE / xFeedrate);
    yDistf *= (MAX_STEPRATE / yFeedrate);
    uint16_t numEntries = (xDistf > yDistf) ? xDistf : yDistf;
    if (numEntries > getStepsAvailable())
    {
        return false; //Not enough room in buffer for this move yet
    }

    //Convert to number steps
    xDist *= X_STEPS_PER_MM;
    yDist *= Y_STEPS_PER_MM;

    //TODO: VERIFY THAT WE ACTUALLY USE STEP RATE BELOW (MAY USE DISTANCE CURRENTLY)

    //Bresenham's algorithm, used once for x and once for y.
    //Basically, where x is traditionally used, we use "z"- one entry in our step buffer.
    //Then, both x and y are incremented when necessary.
    //In other words, we "draw" two simultaneous "lines", where:
    //xSlope = xStepRate/MaxStepRate
    //ySlope = yStepRate/MaxStepRate
    int32_t mx = xDist << 1; // = (x2-x1)*2 = 2*deltaX
    int32_t my = yDist << 1; // = (y2-y1)*2 = 2*deltaY
    //numEntries = 2*deltaZ (z being our independent variable)
    int32_t xErr = mx - xDist;
    int32_t yErr = my - yDist;
    uint8_t step = 0x00;
    for (uint16_t i = 0; i < numEntries; i++)
    {
        step = direction; //Set what direction to move in
        xErr += mx;
        yErr += my;

        if (xErr >= 0)
        {
            step |= X_STEP_BIT_POS; //Tell x to step
            xErr += mx - numEntries;
        }
        if (yErr >= 0)
        {
            step |= Y_STEP_BIT_POS; //Tell y to step
            yErr -= my - numEntries;
        }
        pushToBuffer(step); //Actually queue these steps to buffer
    }
    return true;
}
