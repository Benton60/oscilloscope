#include "main.h"
/*
Voltage Data Points Holder
    queue for the incoming voltages triggered by the interrupt  -> input
    queue for the trigger value                                 -> input
    queue for amplitude value                                   -> input
    queue for time value                                        -> input
    mutex for the spi screen buffer                             -> output
*/
#ifndef VOLTAGE_DATA_POINTS_HOLDER_H_ 
#define VOLTAGE_DATA_POINTS_HOLDER_H_

void set_adc_clock_prescalar(uint16_t horizontal_scale_state);
void ADC_Start_DMA(void *);
void transfer_adc_buffer_to_screen_buffer(uint16_t * current_horizontal_state, uint16_t * adcBuffer, uint8_t * screen_buffer);
void fill_mode2_buffer(uint16_t current_horizontal_state, uint16_t * adcBuffer, uint16_t * adcMode2Buffer);

#endif
