/*
 * amplitude_dial_driver.c
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */

#include "amplitude_dial_driver.h"
#include "main.h"

uint8_t a1;
uint8_t a2;
int16_t amplitude_state = 1100;

int8_t read_amplitude_dial(void)
{
    static uint8_t state = 0;
    static const int8_t table[16] = {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };

    uint8_t a = (GPIOB->IDR >> 14) & 1;
    uint8_t b = (GPIOB->IDR >> 15) & 1;

    state = ((state << 2) | (a << 1) | b) & 0x0F;
    return table[state];
}

int16_t set_amplitude_scale(int8_t up_or_down)
{
	if(up_or_down<0)
	{
		if(amplitude_state==100)
		{
			amplitude_state=200;
		}
		amplitude_state-=100;
	}
		if(up_or_down>0)
	{
		if(amplitude_state==3300)
		{
			amplitude_state=3200;
		}
		amplitude_state+=100;
	}
	return amplitude_state;
}
