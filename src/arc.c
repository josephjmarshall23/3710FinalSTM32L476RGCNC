#include "main.h"
#include "motion.h"
#include "interrupt.h"
#include "pins.h"
#include "arc.h"

//This file has functions to handle arcs (G2/G3 gcode commands).
//Arcs are split into a bunch of short (usually <= 1mm) lines, which are then drawn normally.

float findRadius(float x_dist, float y_dist) //These are distances from arc start pos to arc center pos
{
	return sqrt(x_dist*x_dist + y_dist*y_dist);
}

//clockwise should be 0 for clockwise and 1 for counterclockwise
//I'm worried this function may be an absolute monster.
//It involves a TON of float math, including 2 divs, 2 atan2s, a fabs, a sqrt, and a lot of sin/cos
void writeArc(float x_final_offset, float y_final_offset, float x_center_offset, float y_center_offset, float feedrate, int counterclockwise)
{
	float rad = findRadius(x_center_offset, y_center_offset);
	if (rad < 0.1) //No-op
		return;

	float theta_start = atan2(-y_center_offset, -x_center_offset);
	float theta_end = atan2((y_final_offset - y_center_offset), (x_final_offset - x_center_offset));
	float theta_total = theta_end-theta_start;

	if (!counterclockwise && theta_total > 0) theta_total -= TWO_PI;
	if (counterclockwise && theta_total < 0) theta_total += TWO_PI;

	float arc_length = fabs(theta_total) * rad;

	uint32_t num_segments = (uint32_t) ceil(arc_length / ARC_LINE_LENGTH);

	if (num_segments > ARC_MAX_SEGMENTS)
		num_segments = ARC_MAX_SEGMENTS;

	float delta_theta = theta_total / num_segments;
	float theta_cur;

	float prev_x = 0;
	float prev_y = 0;
	float cur_x, cur_y;

	for (int i = 1; i <= num_segments; i++)
	{
		theta_cur = theta_start + ((float)i * delta_theta); //The current angle we are at relative to arc centerpoint

		cur_x = x_center_offset + rad*cos(theta_cur);
		cur_y = y_center_offset + rad*sin(theta_cur);

		writeLine(cur_x-prev_x, cur_y-prev_y, feedrate);

		prev_x = cur_x;
		prev_y = cur_y;
	}
	return;
}
