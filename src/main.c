#include "init.h"
#include "main.h"
#include "motion.h"
#include "homing.h"
#include "pins.h"
#include "systick.h"
#include "interrupt.h"
#include "arc.h"

int main()
{
	//Procedure:
	/*
	 * Initialize
	 * Test pen lift
	 * Test pen Unlift
	 * Test X positive movement (400 steps)
	 * Test Y positive movement (400 steps)
	 * Test line from (0,0) to (50,0) (feedrate 10 mm/sec or 600 mm/min)
	 * Test line from (50,0) to (50,50)
	 * Test line from (50,50) to (0,0)
	 */

	//Initialize
	system_init();

	enableSteppers();

	penLift();

	homeX();
	homeY();

	penUnlift();

	writeLine(0.0, -150.0, 1800);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 1800);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 8400);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 8400);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 9600);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 9600);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 10800);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 10800);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 12000);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 12000);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 13200);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 13200);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, -150.0, 14400);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 14400);
	writeLine(3.0, 0.0, 1800);

	while(1)
	{
		updateLine();
	}
//	disableSteppers();
}
