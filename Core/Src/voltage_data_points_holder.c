#include "voltage_data_points_holder.h"
#include "app_freertos.h"
#include "spi_display_driver.h"
#include "main.h"


void set_adc_clock_prescalar(uint16_t * horizontal_scale_state);


void ADC_Start_DMA(void * adcBuffer) {
    LL_ADC_Enable(ADC1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1)){}

    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_CHANNEL_1,
        LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
        (uint32_t) adcBuffer,
        LL_DMA_DIRECTION_PERIPH_TO_MEMORY
    );

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_BUFFER_SIZE);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    LL_ADC_REG_StartConversion(ADC1);
}

void transfer_adc_buffer_to_screen_buffer(void * adcBuffer_null, void * screen_buffer_null) {
    uint16_t * adcBuffer = adcBuffer_null;
    uint8_t * screen_buffer = screen_buffer_null;
    uint16_t vertical_scale = 3500;             // 100 - 3300
    uint16_t horizontal_scale_state = 100;       // 1 - 11           NOTE -- the horizontal scale is stored as a state NOT A VALUE
    uint16_t trigger_level  = 1000;             // 0 - 3300
    uint16_t trigger_direction = 1;             // 1 - up, 0 - down


    // Only 120 pixels can be shown on the screen at once...
    // my adc DMA buffer can hold 256...
    // the extra DMA space is so that the trigger can "shift"...
    // where from the DMA buffer the data is being pulled from.
    int trigger_offset = 0;
    for(int i = 0; i < 145; i++){   // will time out and just draw the screen if no trigger is detected
        if((adcBuffer[i] <= trigger_level && adcBuffer[i+1] > trigger_level) && trigger_direction){             //checks smaller than, bigger than, rising
            break;
        } else if((adcBuffer[i] >= trigger_level && adcBuffer[i+1] < trigger_level) && ~trigger_direction) {    //checks bigger than, smaller than, falling
            break;
        }
        trigger_offset++;
    }
    for(int i = trigger_offset; i < SCREEN_COLUMNS + trigger_offset; i++) {
        int pixels_up = adcBuffer[i] * SCREEN_ROWS / 4096 * 3300 / vertical_scale;
        int page = pixels_up / 8;
        int pixels_up_per_page = pixels_up % 8;
        for(int j = 0; j < 8; j++) {
            if(j == page){
                screen_buffer[j * SCREEN_COLUMNS + i - trigger_offset] = (1<<pixels_up_per_page);
            } else {
                screen_buffer[j * SCREEN_COLUMNS + i - trigger_offset] = 0x00;
            }
        }
    }
    set_adc_clock_prescalar(&horizontal_scale_state); //changes the pre-scalar based on horizontal_scale 
    LL_ADC_REG_StartConversion(ADC1);
}

void set_adc_clock_prescalar(uint16_t * horizontal_scale_state) {
    switch (*horizontal_scale_state) {
        case 1:
            break;
        case 2: 
            break;
        case 3:
            break;
        case 4: 
            break;
        case 5:
            break;
        case 6: 
            break;
        default:
            break;
    }
}

