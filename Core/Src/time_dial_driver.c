#include "time_dial_driver.h"
#include "main.h"

uint8_t t1;
uint8_t t2;
int16_t time_state = 100;

int8_t read_time_dial()
{
	uint16_t t1_cur = GPIOC -> IDR & 1;
		uint16_t t2_cur = (GPIOC -> IDR >> 0) & 1;
		if(t1_cur == 0 && t1 == 1)
		{
			t1 = 0;
			if(t2_cur==1)
			{
				return 1;
			}
			else
			{
				return -1;
			}
			return 1;
		}
		t1 = t1_cur;
		t2 = t2_cur;
		return 0;
}
int16_t set_time_scale(int8_t up_or_down)
{
	if(up_or_down==-1)
		{
			if(time_state<=100)
			{
				time_state=50;
			}
			time_state*=2;
		}
	if(up_or_down==1)
		{
			if(time_state>100000)
			{
				time_state=200000;
			}
			time_state/=2;
		}
		return time_state;
}
