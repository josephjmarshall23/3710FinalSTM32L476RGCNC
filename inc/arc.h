#pragma once

#define TWO_PI 6.2831853071795864769 //Probably way more precise than float math can handle, but alas

void writeArc(float x_final_offset, float y_final_offset, float x_center_offset, float y_center_offset, float feedrate, int clockwise);
