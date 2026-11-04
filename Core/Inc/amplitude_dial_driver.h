/*
 * amplitude_dial_driver.h
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */
/*
Amplitude Dial Driver
    queue for Voltage Data Points Holder                        -> output
    wueue for Char Display Driver                               -> output
*/
#ifndef SRC_AMPLITUDE_DIAL_DRIVER_H_
#define SRC_AMPLITUDE_DIAL_DRIVER_H_

#include "main.h"

int8_t read_amplitude_dial();
int16_t set_amplitude_scale(int8_t up_or_down);

#endif /* SRC_AMPLITUDE_DIAL_DRIVER_H_ */
