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

int8_t read_amplitude_dial()
{
	uint16_t a1_cur = GPIOC -> IDR & 1;
	uint16_t a2_cur = (GPIOC -> IDR >> 0) & 1;
	if(a1_cur == 0 && a1 == 1)
	{
		a1 = 0;
		if(a2_cur==1)
		{
			return 1;
		}
		else
		{
			return -1;
		}
		return 1;
	}
	a1 = a1_cur;
	a2 = a2_cur;
	return 0;
}

int16_t set_amplitude_scale(int8_t up_or_down)
{
	if(up_or_down==-1)
	{
		if(amplitude_state==0)
		{
			amplitude_state=100;
		}
		amplitude_state-=100;
	}
	if(up_or_down==1)
	{
		if(amplitude_state==3300)
		{
			amplitude_state=3200;
		}
		amplitude_state+=100;
	}
	return amplitude_state;
}
