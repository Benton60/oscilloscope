
#include "time_dial_driver.h"
#include "main.h"

void read_time_dial(int * time_state){
    uint8_t state = 0;
    static const int8_t table[16] = {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };
    uint8_t a = (GPIOB->IDR >> 12) & 1;
    uint8_t b = (GPIOB->IDR >> 13) & 1;
    //State calculation
    state = ((state << 2) | (a << 1) | b) & 0x0F;
    *time_state += table[state]; //Benton added this line. Because the time_state variable is being switched to holding a state rather than
                                //the scale value this function doesn't need to return anything. 
                                //I did this because scaling horizontally depends on the pre-scalar values available which aren't described by any formula. 
                                //As such a "lookup table" will be used to get the value we display to the screen instead of storing the value as the "state".
    if(*time_state == -1) {
        *time_state = 0;        //can't let the state drop below zero obv.
    } else if(*time_state == 11) {
        *time_state = 10;
    }
    //return table[state];
}

int get_time_scale(int time_state) {
    // the slowest pre-scalar available, 10, corresponds to 1 period of a 1kHz wave or 1000 u.s.
    // the other values stored in the table are calculated based on this value. 
    // these values should be changed b4 presentation to exactly match the true frequencies they display TODO
    int lookup_table[11] = {100, 200, 400, 600, 800, 1000, 2000, 4000, 6000, 8000, 10000};
    //these values are in micro seconds aka u.s.
    return lookup_table[time_state];
    
}	


