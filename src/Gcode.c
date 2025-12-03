#include "stm32l476xx.h"
#include "string.h"
#include "Gcode.h"
#include "UART.h"
#include "main.h"
#include "interrupt.h"
#include "motion.h"
#include "stm32l476xx.h"
#include "string.h"
#include "Gcode.h"
#include "UART.h"
#include "init.h"
#include "pins.h"
#include "arc.h"
#include "systick.h"
#include "homing.h"
#include <stdlib.h>
#include <stdio.h>

void Gcode_parser(void) {

  char command[40][15]; //array of 40 strings of 15 chars
	uint32_t element_count = 0;
	uint32_t char_count = 0;
	float feedrate = 2500;
	float cur_x = 0.01;
	float cur_y = 0.01;

	char read_char = '#'; //random value so its not NULL
	
	// Skip leftover \n or \r from last transmission (can be stored in FIFO still)
	do {
		USART_Read(USART2, (uint8_t*)&read_char, 1);
	} while (read_char == '\r' || read_char == '\n');
	while((read_char != '\r') &&(read_char != '\n')) {//while we aren't on a comment or at the end of a line get data
		
    //get single char from uart
		USART_Read(USART2, (uint8_t*)&read_char, 1);
		//USART_Write(USART2, (uint8_t*)&read_char, 1); //debug for serial terminal
		
		//use space char as a deliminator to save each chunk of the command in an array as separate elements
		if(read_char == ' '){
			command[element_count][char_count] = '\0'; //Null-terminate
			element_count++;
			char_count = 0;
		}
		else {
			command[element_count][char_count] = read_char;
			char_count++;
		}
	}
	command[element_count][char_count] = '\0'; //Null-terminate

	
	char str[50];
	sprintf(str, "Cur: (%05d, %05d)\r\n", (int)cur_x*1000, (int)cur_y*1000);
	USART_Write(USART2, (unsigned char*)str, strlen(str));  //serial testing line
	float to_relative_x = atof(command[1]+1) - cur_x; //Parse after 'X'
	float to_relative_y = atof(command[2]+1) - cur_y; //Parse after 'Y'
	sprintf(str, "Rel: (%05d, %05d)\r\n", (int)to_relative_x*1000, (int)to_relative_y*1000);
	USART_Write(USART2, (unsigned char*)str, strlen(str));  //serial testing line
	
	//switch to call correct command
	if (command[0][0] == 'G')
	{
		switch(command[0][2]) { //switch on gcode opcode last number
			case '0': //move command
				if(command[1][0] == 'Z'){ //we are moving the pen
					if(command[1][1] == '-') { //z move is negative meaning lower the pen
						writeAction(PEN_DROP);
						USART_Write(USART2, (unsigned char*)"Pen Down\n\r", 10);  //serial testing line
					}
					else {	//z move is positive meaning raise the pen
						writeAction(PEN_LIFT);
						USART_Write(USART2, (unsigned char*)"Pen Up\n\r", 8);  //serial testing line
					}
				}
				else {  //we are drawing a line
					writeLine(to_relative_x, to_relative_y, feedrate);
					USART_Write(USART2, (unsigned char*)"WriteLine\n\r", 11);  //serial testing line
				}

				break;

			case '1': //draw line command
				if(command[1][0] == 'Z'){ //we are moving the pen
					if(command[1][1] == '-') { //z move is negative meaning lower the pen
						writeAction(PEN_DROP);
						USART_Write(USART2, (unsigned char*)"Pen Down\n\r", 10);  //serial testing line
					}
					else {	//z move is positive meaning raise the pen
						writeAction(PEN_LIFT);
						USART_Write(USART2, (unsigned char*)"Pen Up\n\r", 8);  //serial testing line
					}
				}
				else {  //we are drawing a line
					writeLine(to_relative_x, to_relative_y, feedrate);
					USART_Write(USART2, (unsigned char*)"WriteLine\n\r", 11);  //serial testing line
				}

				break;

			case '2': //arc clockwise
				writeArc(to_relative_x, to_relative_y, atof(command[4]+1), atof(command[5]+1), feedrate, 1); //only first two need to be modified, others are already relative 1 at the end means cw
				USART_Write(USART2, (unsigned char*)"Arc CW\n\r", 8);  //serial testing line
				break;

			case '3': //arc ccw
				writeArc(to_relative_x, to_relative_y, atof(command[4]+1), atof(command[5]+1), feedrate, 0); //0 means ccw
				USART_Write(USART2, (unsigned char*)"Arc CCW\n\r", 9);  //serial testing line
				break;

			case '8': //home
				writeAction(PEN_LIFT);
				writeAction(HOME_X);
				writeAction(HOME_Y);
				writeAction(PEN_DROP);
				cur_x = X_HOMING_POS;
				cur_y = Y_HOMING_POS;
				USART_Write(USART2, (unsigned char*)"Home\n\r", 6);  //serial testing line
				USART_Write(USART2, (unsigned char*)"ok\n\r", 4);  //serial testing
				break;

			default: //do nothing with command and acknowledge to get the next one
				//acknowledge();
	//			USART_Write(USART2, (unsigned char*)"Default\n\r", 9);  //serial testing line
				break;
		}
	} else if(command[0][0] == '$')
	{
		switch(command[0][1])
		{
		case 'I':
			USART_Write(USART2, (unsigned char*)"[VER:1.1h.20190825:]\n\r", 22);  //serial testing
			USART_Write(USART2, (unsigned char*)"[OPT:V,15,128]\n\r", 16);  //serial testing
			USART_Write(USART2, (unsigned char*)"ok\n\r", 4);  //serial testing
			break;
		case '$':
			USART_Write(USART2, (unsigned char*)"$0=10\n\r", 7);  //serial testing
			USART_Write(USART2, (unsigned char*)"ok\n\r", 4);  //serial testing
			break;
		default:
			break;
		}
	}
	element_count = 0;
	char_count = 0;
  }

void startupString() //Required for gcode sender to assume this is Grbl firmware and send appropriate gcodes
{
	USART_Write(USART2, (unsigned char*)"Grbl 1.1h ['$' for help]\r\n", 26);  //serial testing line
}

 void acknowledge()
 {
		USART_Write(USART2, (unsigned char*)"ok\r\n", 4);  //serial testing line
		char str[15];
		sprintf(str, "%04d\r\n", LINE_QUEUE_MAX-getLineQueueSpace());
		USART_Write(USART2, (unsigned char*)str, 6);  //serial testing line

 }
