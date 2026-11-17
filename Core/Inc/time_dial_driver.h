/*
Time Dial Driver
    queue for Voltage Data Points Holder                        -> output
    queue for Char Display Driver                               -> output
*/
#ifndef TIME_DIAL_DRIVER_H_   
#define TIME_DIAL_DRIVER_H_

#include "main.h"

#define NUM_STATES 10
#define NUM_MODE1_STATES 6
#define NUM_MODE2_STATES 4

void read_time_dial(int * time_state); //we need to modify time_state so we pass a reference
int get_time_scale();

#endif 
