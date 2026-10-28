/*
 * char_display_driver.c
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */

#include "char_display_driver.h"
#include "main.h"
#include "app_freertos.h"

void draw_char_screen()
{
	return; /*TODO*/
}

void init_char_display()
{
	return;
}

void command(char i)
{
	GPIOB -> ODR = (GPIOB -> ODR & ~0xFF) | i;
	GPIOB -> ODR &= ~(1<<8);
	GPIOB -> ODR &= ~(1<<9);
	GPIOB -> ODR |= (1<<10);
	osDelay(1);
	GPIOB -> ODR &= ~(1<<10);
}

void write(char i)
{
	GPIOB -> ODR = (GPIOB -> ODR & ~0xFF) | i;
	GPIOB -> ODR |= (1<<8);
	GPIOB -> ODR &= ~(1<<9);
	GPIOB -> ODR |= (1<<10);
	osDelay(1);
	GPIOB -> ODR &= ~(1<<10);
}

void init()
{
	GPIOB -> ODR &= ~(1<<10);
	osDelay(45);
	command(0x30);
	osDelay(5);
	command(0x30);
	osDelay(1);
	command(0x30);
	osDelay(1);
	command(0x38);
	command(0x10);
	command(0x0c);
	command(0x06);
	command(0x01);
	osDelay(2);
}
