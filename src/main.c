#include "init.h"
#include "main.h"
#include "motion.h"
#include "homing.h"
#include "pins.h"
#include "systick.h"
#include "interrupt.h"

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

	homeX();
	homeY();

	disableSteppers();

	while (1);

	//Test pen lift/unlift
	penLift();
//	penUnlift();

	//Set direction to X+, Y+
	setDirection(0);

	//Test X positive movement (400 steps)
	for (int i = 0; i < 1600; i++)
	{
		stepX();
		for (int j = 0; j < 200; j++);
	}


	//Test Y positive movement (400 steps)
	for (int i = 0; i < 1600; i++)
	{
		stepY();
		for (int j = 0; j < 200; j++);
	}

	penUnlift();
	penLift();
	penUnlift();

	penLift();
	penUnlift();

	penLift();
	penUnlift();



	//Test 3 lines
	writeLine(0.0, 0.0, 50.0, 0.0, 600);
	writeLine(50.0, 0.0, 50.0, 50.0, 600);
	writeLine(50.0, 50.0, 0.0, 0.0, 600);

	while(1)
	{
		updateLine();
	}
//	disableSteppers();
}
