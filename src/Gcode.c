#include "stm32l476xx.h"
#include "string.h"
#include "Gcode.h"
#include "UART/UART.h"


void Gcode_parser(void) {

  uint8_t command[40][15]; //array of 40 strings of 15 chars
	uint32_t element_count = 0;
	uint32_t char_count = 0;

	uint8_t read_char = '#'; //random value so its not NULL
	
  while((read_char != '\r') &&(read_char != '\n') && (read_char != '(')) {//while we aren't on a comment or at the end of a line get data
		
    //get single char from uart
		USART_Read(USART2, &read_char, 1);
		USART_Write(USART2, &read_char, 1); //debug for serial terminal
		
		//use space char as a deliminator to save each chunk of the command in an array as separate elements
		if(read_char == ' '){
			element_count++;
			char_count = 0;
		}
		else {
			command[element_count][char_count] = read_char;
			char_count++;
		}
	}
	
	
	
	
	//switch to call correct command
	switch(command[0][2]) { //switch on gcode opcode last number
		
		case '0': //move command
			
			if(command[1][0] == 'Z'){ //we are moving the pen
				if(command[1][1] == '-') { //z move is negative meaning lower the pen
					//penUnlift();
					USART_Write(USART2, (unsigned char*)"Pen Down\n\r", 10);  //serial testing line
				}
				else {	//z move is positive meaning raise the pen
					//penLift();
					USART_Write(USART2, (unsigned char*)"Pen Up\n\r", 8);  //serial testing line
				}
			}
			else {  //we are drawing a line
				//writeLine(relative x, relative y, feedrate);
				USART_Write(USART2, (unsigned char*)"WriteLine\n\r", 11);  //serial testing line
			}
			
			break;
			
			
		case '1': //draw line command
			
			if(command[1][0] == 'Z'){ //we are moving the pen
				if(command[1][1] == '-') { //z move is negative meaning lower the pen
					//penUnlift();
					USART_Write(USART2, (unsigned char*)"Pen Down\n\r", 10);  //serial testing line
				}
				else {	//z move is positive meaning raise the pen
					//penLift();
					USART_Write(USART2, (unsigned char*)"Pen Up\n\r", 8);  //serial testing line
				}
			}
			else {  //we are drawing a line
				//writeLine(relative x, relative y, feedrate);
				USART_Write(USART2, (unsigned char*)"WriteLine\n\r", 11);  //serial testing line
			}
		
			break;
		
		
		case '2': //arc clockwise
			//writeArc(x relative end, y relative end, x center, y center, feedrate, 1); //only first two need to be modified, others are already relative 1 at the end means cw
			USART_Write(USART2, (unsigned char*)"Arc CW\n\r", 8);  //serial testing line
			break;
		
		
		case '3': //arc ccw
			//writeArc(x relative end, y relative end, x center, y center, feedrate, 0); //0 means ccw
			USART_Write(USART2, (unsigned char*)"Arc CCW\n\r", 9);  //serial testing line
			break;
		
		
		case '8': //home
			//penLift();
			//homeX();
			//homeY();
			//penUnlift();
			USART_Write(USART2, (unsigned char*)"Home\n\r", 6);  //serial testing line
			break;
		
		
		default: //do nothing with command and acknowledge to get the next one
			//acknowledge();
			USART_Write(USART2, (unsigned char*)"Default\n\r", 9);  //serial testing line
			break;
		
	}
	
	
	
	
		
		
		
    
		
  }