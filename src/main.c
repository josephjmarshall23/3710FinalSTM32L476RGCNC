#include "init.h"
#include "main.h"
#include "motion.h"
#include "homing.h"
#include "pins.h"
#include "systick.h"
#include "interrupt.h"
#include "arc.h"
#include "Gcode.h"
#include "parser.h"
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

	//Enable stepper motors and home axes
	machine_init();

	parse_loop(); //Endless loop that parses and executes commands
//	disableSteppers();
}
