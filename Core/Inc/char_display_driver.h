/*
 * char_display_driver.h
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */
/*
Char Display Driver
    queue for Voltage Dial Driver                               -> input
    queue for Time Dial Driver                                  -> input
*/
#ifndef SRC_CHAR_DISPLAY_DRIVER_H_
#define SRC_CHAR_DISPLAY_DRIVER_H_

void draw_char_screen();
void init_char_display();
// Initialization Functions
void command(char i);
void write(char i);
void init();
void myDelay(int ms);
#endif /* SRC_CHAR_DISPLAY_DRIVER_H_ */
