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

	//Test pen lift/unlift
	penLift();
//	penUnlift();

	homeX();
	//Don't home Y (not working great yet)

//
//	penLift();
	penUnlift();
//
//	penLift();
//	penUnlift();
//
//
//
//	//Test 3 lines
	writeLine(0.0, 0.0, 50.0, 50.0, 600);
	writeLine(50.0, 50.0, 50.0, 100.0, 600);

	writeArc(50.0, 100.0, 50.0, 150.0, 0.0, 25.0, 600, 1);
	writeArc(50.0, 150.0, 50.0, 200.0, 0.0, 25.0, 600, 0);
//
	while(1)
	{
		updateLine();
	}
//	while(1)
//	{
//		updateLine();
//	}
//	disableSteppers();
}
