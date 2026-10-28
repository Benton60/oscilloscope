/*
 * spi_display_driver.c
 *
 *  Created on: Oct 11, 2026
 *      Author: ethanwatkins
 */
#include "main.h"
#include "spi_display_driver.h"
#include "app_freertos.h"
void draw_SPI_screen()
{
	return; /*TODO*/
}

void init_spi_screen(){
    //SPI1 : PA1 for CLK, PA7 for MOSI, PC7 for Select, PC8 for Data/Command, PC9 for Reset
    LL_SPI_Enable(SPI1);
    
    GPIOC->ODR &= ~(1<<9);  // drive the screen reset low aka reset the screen before initialization
    osDelay(10);            // Wait for the screen to register the reset
    GPIOC->ODR |= (1<<9);   // drive reset back high 

    //Initialization sequence, I can't be bothered to explain what each byte does.
    uint8_t init[] = {0x40, 0xa1, 0xc0, 0xa4, 0xa6, 0xa2, 0x2f, 0x27, 0x81, 0x10, 0xfa, 0x90, 0xaf}; 
    for(int i = 0; i < 13; i++){
        send_command(init[i]);
    }
 
}

void clear_screen() {
    for(int j = 0; j < 8; j++){
        set_page(j);
        set_column(0x00);
        for(int i = 0; i < 120; i++) {
            send_byte(0x00);
        }
    }
}

void set_page(uint8_t page) {
   send_command(page | 0xb0); 
}

void set_column(uint8_t column) {
    send_command(column | 0xf0);
    send_command((column>>4) | 0x1f);
}

void send_command(uint8_t command) {
    GPIOC->ODR &= ~(1<<8);  //these are all commands
    send_byte(command);
    GPIOC->ODR |= (1<<8);   //We have finished sending command
}

void send_byte(uint8_t byte) {
    GPIOC->ODR &= ~(1<<7);
    LL_SPI_TransmitData8(SPI1, byte);
    for(int j = 0; j < 1234; j++){  // This is one of the reasons why all byte sends should be sent through the send_bytes function. It abstracts away the need for "readiness" checks.
        if(!LL_SPI_IsActiveFlag_BSY(SPI1))
            break;
    }
    GPIOC->ODR |= (1<<7);
}
