#include "init.h"
#include "main.h"
#include "motion.h"
#include "homing.h"
#include "pins.h"
#include "systick.h"
#include "interrupt.h"
#include "arc.h"
#include "Gcode.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

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
	writeAction(PEN_LIFT);
	writeAction(PEN_DROP);
	writeAction(HOME_X);
	writeAction(HOME_Y);
	writeAction(ENABLE_STP);
	writeAction(DISABLE_STP);
	writeLine(0.0, -150.0, 1800);
	writeLine(3.0, 0.0, 1800);
	writeLine(0.0, 150.0, 1800);
	writeLine(3.0, 0.0, 1800);

	while(1)
	{
		updateLine();
		if (is_room_in_buffer)
		{
			writeLine(0.0, -150.0, 1800);
			writeLine(3.0, 0.0, 1800);
			writeLine(0.0, 150.0, 1800);
			writeLine(-3.0, 0.0, 1800);
			writeAction(PEN_LIFT);
//			acknowledge();
//			Gcode_parser();
//			for(int j = 0; j < 100; j++);
		}

	}
//	disableSteppers();
}
