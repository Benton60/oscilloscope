/*
 * trigger_dial_driver.h
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */
/*
Trigger Dial Driver
    queue for Voltage Data Points Holder                        -> output
*/
#ifndef SRC_TRIGGER_DIAL_DRIVER_H_
#define SRC_TRIGGER_DIAL_DRIVER_H_

#include "main.h"

enum btn_status {RISE, HIGH, H_BOUNCE_DELAY, FALL, LOW, L_BOUNCE_DELAY};

int8_t read_trigger_buttons();
void get_trigger_value();

#endif /* SRC_TRIGGER_DIAL_DRIVER_H_ */
