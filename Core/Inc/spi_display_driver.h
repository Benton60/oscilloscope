/*
 * spi_display_driver.h
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */
/*
SPI Display Driver
    mutex for spi screen buffer                                 -> input
*/
#ifndef SRC_SPI_DISPLAY_DRIVER_H_
#define SRC_SPI_DISPLAY_DRIVER_H_

void draw_SPI_screen();

void send_command(uint8_t command);

void init_spi_screen();

void set_column(uint8_t column);

void set_page(uint8_t page);

void send_byte(uint8_t byte);

void clear_screen(); 
#endif /* SRC_SPI_DISPLAY_DRIVER_H_ */
