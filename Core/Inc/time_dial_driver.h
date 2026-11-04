/*
Time Dial Driver
    queue for Voltage Data Points Holder                        -> output
    wueue for Char Display Driver                               -> output
*/
#ifndef TIME_DIAL_DRIVER_H_   
#define TIME_DIAL_DRIVER_H_

#include "main.h"

int8_t read_time_dial();
int32_t set_time_scale(int8_t up_or_down);

#endif 
