#include "time_dial_driver.h"
#include "main.h"

uint8_t t1;
uint8_t t2;
int32_t time_state = 100000;

int8_t read_time_dial(void)
{
    static uint8_t state = 0;
    static const int8_t table[16] = {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };

    uint8_t a = (GPIOB->IDR >> 12) & 1;
    uint8_t b = (GPIOB->IDR >> 13) & 1;

    state = ((state << 2) | (a << 1) | b) & 0x0F;
    return table[state];
}

int32_t set_time_scale(int8_t up_or_down)
{
	if(up_or_down==-1)
		{
			if(time_state<=100) time_state=200;
			time_state = time_state / 2;
		}
	if(up_or_down==1)
		{
			if(time_state>=100000) time_state=50000;
			time_state = time_state * 2;
		}
		return time_state;
}
