//#include "main.h"
//#include "interrupt.h"
//#include "motion.h"
//#include <stdint.h>
//
//void accelLine(float x_dist, float y_dist, float start_feedrate, float end_feedrate, float max_feedrate, float acceleration)
//{
//	//Calculate what portion will be accel and what will be decel
//	float len = sqrtf(x_dist*x_dist+y_dist*y_dist);
//	float accel_time = (max_feedrate - start_feedrate) / acceleration;
//	float decel_time = (max_feedrate - end_feedrate) / acceleration;
//	float cruise_len;
//
//	if ((accel_len + decel_len) >= total_len)
//	{
//		//Accelerate for half the time, decelerate half the time
//		cruise_len = 0;
//	}
//	else
//	{
//		//Trapezoids are my least favorite shape
//		cruise_len = total_len - accel_len - decel_len;
//	}
//
//
//	//Write out accel and decel lines
//	for (int i = 0; i < accel_len; i++)
//	{
//		//Accelerate
//		feedrate += 50;
//		writeLine();
//	}
//	for (int i = 0; i < cruise_len; i++)
//	{
//		writeLine();
//	}
//	for (int i = 0; i < decel_len; i++)
//	{
//		feedrate -= 50;
//		writeLine();
//	}
//
//}
