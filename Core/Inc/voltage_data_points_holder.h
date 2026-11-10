
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

void ADC_Start_DMA(void *);
void transfer_adc_buffer_to_screen_buffer(void * adcBuffer, void * screen_buffer);

#endif
