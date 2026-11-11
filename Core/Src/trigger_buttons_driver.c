/*
 * trigger_dial_driver.c
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */

#include <trigger_buttons_driver.h>
#include "main.h"

int8_t read_trigger_buttons()
{
	static enum btn_status p = HIGH;
	switch (p)
	{
	case HIGH:
		if (GPIOA -> IDR & (1<<4))
		{
			break;
		}
		else
		{
			p = H_BOUNCE_DELAY;
		}
		break;
	case RISE:
		p = HIGH;
		break;
	case H_BOUNCE_DELAY:
		if (GPIOA -> IDR & (1<<4))
		{
			p = HIGH;
		}
		else
		{
			p = FALL;
		}
		break;
	case FALL:
		p = LOW;
		break;
	case LOW:
		if (GPIOA -> IDR & (1<<4))
		{
			p = L_BOUNCE_DELAY;
		}
		else
		{
			break;
		}
		break;
	case L_BOUNCE_DELAY:
		if (GPIOA -> IDR & (1<<4))
		{
			p = RISE;
		}
		else
		{
			p = LOW;
		}
		break;
	}
	static enum btn_status n = HIGH;
	switch (n)
	{
	case HIGH:
		if (GPIOA -> IDR & (1<<5))
		{
			break;
		}
		else
		{
			n = H_BOUNCE_DELAY;
		}
		break;
	case RISE:
		n = HIGH;
		break;
	case H_BOUNCE_DELAY:
		if (GPIOA -> IDR & (1<<5))
		{
			n = HIGH;
		}
		else
		{
			n = FALL;
		}
		break;
	case FALL:
		n = LOW;
		break;
	case LOW:
		if (GPIOA -> IDR & (1<<5))
		{
			n = L_BOUNCE_DELAY;
		}
		else
		{
			break;
		}
		break;
	case L_BOUNCE_DELAY:
		if (GPIOA -> IDR & (1<<5))
		{
			n = RISE;
		}
		else
		{
			n = LOW;
		}
		break;
	}
	if (p == FALL)
		{
			return 100;
		}
	else
	{
		if (n == FALL)
		{
			return -100;
		}
		else
		{
			return 0;
		}
	}
}

void get_trigger_value()
{
	return; /*TODO*/
}


