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

#define SCREEN_COLUMNS 102
#define SCREEN_ROWS 64
#define SCREEN_PAGES 8

void draw_spi_screen(uint8_t * screen_buffer);

void send_command(uint8_t command);

uint8_t * init_spi_screen();

void set_column(uint8_t column);

void set_page(uint8_t page);

void send_byte(uint8_t byte);

void clear_screen(); 
#endif /* SRC_SPI_DISPLAY_DRIVER_H_ */
