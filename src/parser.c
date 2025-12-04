#include "stm32l476xx.h"
#include "main.h"
#include "interrupt.h"
#include "motion.h"
#include "string.h"
#include "UART.h"
#include "init.h"
#include "pins.h"
#include "arc.h"
#include "systick.h"
#include "homing.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

#define GCODE_MAX_LEN 100

float cur_x, cur_y;
uint8_t pen_active; //1 means down, 0 means lifted

void error()
{
	uart_write("error:\n", 7);
}

void ack()
{
	uart_write("ok\n", 3);
}

//Given a string and delimiter (X, Y, Z, etc), parse the float value after the delimiter
float parse(char *cmd, char delim)
{
	uint32_t pos = 0;
	//Advance to delimiter character
	while (cmd[pos] && (cmd[pos] != delim)) pos++;
	if (!cmd[pos]) //Hit end of string (null-terminated)
	{
		return 0.0f;
	}
	return atof(cmd+pos+1);
}

//Contains an endless loop
void parse_loop(void)
{
//	char read_char; //Temporary function-check loop (works)
//	while(1)
//	{
//		if (uart_available())
//		{
//			read_char = uart_read();
//			uart_write(&read_char, 1);
//		}
//	}

	uart_enable(); //Now we want to receive data, make sure we can!

	char read_char = 0;
	char command[GCODE_MAX_LEN];
	uint32_t num_chars = 0;
	cur_x = X_HOMING_POS;
	cur_y = Y_HOMING_POS;
	pen_active = 1; //Pen starts down
	while (1) //Endless loop
	{
		updateLine(); //Necessary function to keep track of spot in movement queue

		if (uart_available())
		{
			read_char = uart_read(); //Grab next byte
			if (read_char == '\n') //Newline indicates command is finished
			{
				command[num_chars] = '\0'; //Null-terminate
				parse_command(command);
				num_chars = 0;
				while (!is_room_in_buffer)
				{
					updateLine();
				}
				ack(); //We now have room for the next command
			}
			else
			{
				if (num_chars+1 < GCODE_MAX_LEN)
				{
					command[num_chars++] = read_char;
				}
				else
				{
					uart_write("error:2Long\n", 12);
					num_chars = 0; //Restart
				}
			}
		}
	}
}


void parse_command(char *cmd)
{
	uint32_t pos = 0;

	//Ignore anything before first delimiter (G, M, etc)
	while (!is_delimiter(cmd[pos]) && cmd[pos]) pos++;
	if (!cmd[pos]) return; //No command present

	switch (cmd[pos])
	{
		case 'G':
			g_command(cmd+pos+1); //Pass everything after the G
			break;
		case 'M':
			m_command(cmd+pos+1); //Pass everything after the M
			break;
		default:
			uart_write("error:CmdNotRecognized\n", 23);
			break;
	}
}

/*
 * Currently supported commands:
 * G0/G00	(linear move)
 * G1/G01	(linear move)
 * G2/G02	(arc, clockwise)
 * G3/G03	(arc, counterclockwise)
 * G28		(home)
 *
 * Currently supported axes/delimiters:
 * X
 * Y
 * Z
 * F
 * I
 * J
 */
void g_command(char *cmd)
{
	static float parsed_x, parsed_y, parsed_z, parsed_f, parsed_i, parsed_j;

	//Get opcode
	uint32_t num = atoi(cmd);
	switch (num)
	{
	case 0:
			//Actually, we treat 0 and 1 the same, so fall through to 1 here!
	case 1:
		parsed_x = parse(cmd, 'X') - cur_x; //Relative X (absolute - current)
		parsed_y = parse(cmd, 'Y') - cur_y; //Relative Y (absolute - current)
		parsed_z = parse(cmd, 'Z'); //We just care about the sign on Z
		parsed_f = parse(cmd, 'F'); //Feedrate

		if ((parsed_z > 0.0) && pen_active) writeAction(PEN_LIFT);
		if ((parsed_z < 0.0) && !pen_active) writeAction(PEN_DROP);
		if (parsed_f < MIN_FEEDRATE) parsed_f = DEFAULT_FEEDRATE;
		writeLine(parsed_x, parsed_y, parsed_f);
		cur_x += parsed_x;
		cur_y += parsed_y;
		break;
	case 2:
			//Similarly, we treat 2 and 3 the same, so fall through to 3 here
	case 3:
		parsed_x = parse(cmd, 'X') - cur_x; //Relative X (absolute - current)
		parsed_y = parse(cmd, 'Y') - cur_y; //Relative Y (absolute - current)
		parsed_i = parse(cmd, 'I'); //Center offset X. Already relative.
		parsed_j = parse(cmd, 'J'); //Center offset Y. Already relative.
		parsed_z = parse(cmd, 'Z'); //We just care about the sign on Z
		parsed_f = parse(cmd, 'F'); //Feedrate

		if ((parsed_z > 0.0) && pen_active) writeAction(PEN_LIFT);
		if ((parsed_z < 0.0) && !pen_active) writeAction(PEN_DROP);
		if (parsed_f < MIN_FEEDRATE) parsed_f = DEFAULT_FEEDRATE;
		writeArc(parsed_x, parsed_y, parsed_i, parsed_j, parsed_f, (num-2));
		cur_x += parsed_x;
		cur_y += parsed_y;
		break;
	case 28:
		writeAction(PEN_LIFT | HOME_X | HOME_Y | PEN_DROP);
		cur_x = X_HOMING_POS;
		cur_y = Y_HOMING_POS;
		pen_active = 1;
		break;
	default:
		uart_write("error:CmdNotRecognized\n", 23);
		break;
	}
}

/*
 * Currently, we don't support M-codes, but we don't throw an error.
 */
void m_command(char *cmd)
{
	return;
}







































