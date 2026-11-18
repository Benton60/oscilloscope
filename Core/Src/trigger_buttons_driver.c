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
	switch (p) {
	case HIGH:
		if (!(GPIOA -> IDR & (1<<4))) {
			p = H_BOUNCE_DELAY;
		}
		break;
	case RISE:
		p = HIGH;
		break;
	case H_BOUNCE_DELAY:
        p = (GPIOA -> IDR & (1<<4)) ? RISE : FALL;
        break;
	case FALL:
		p = LOW;
		break;
	case LOW:
		if (GPIOA -> IDR & (1<<4)) {
			p = L_BOUNCE_DELAY;
		} 
		break;
	case L_BOUNCE_DELAY:
        p = (GPIOA -> IDR & (1<<4)) ? RISE : LOW;
        break;
	}
	static enum btn_status n = HIGH;
	switch (n) {
	case HIGH:
		if (!(GPIOA -> IDR & (1<<5))) {
			n = H_BOUNCE_DELAY;
		}
		break;
	case RISE:
		n = HIGH;
		break;
	case H_BOUNCE_DELAY:
        n = (GPIOA -> IDR & (1<<5)) ? HIGH : FALL;
		break;
	case FALL:
		n = LOW;
		break;
	case LOW:
		if (GPIOA -> IDR & (1<<5)) {
			n = L_BOUNCE_DELAY;
		} 
		break;
	case L_BOUNCE_DELAY:
        n = (GPIOA -> IDR & (1<<5)) ? RISE : LOW;
		break;
	}
	if (p == FALL) {
		return 100;
	} else {
		return (n == FALL) ? -100 : 0;
	}
}
