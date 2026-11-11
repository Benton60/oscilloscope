#include "voltage_data_points_holder.h"
#include "app_freertos.h"
#include "spi_display_driver.h"
#include "main.h"

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

void transfer_adc_buffer_to_screen_buffer(uint16_t * current_horizontal_state, uint16_t * adcBuffer, uint8_t * screen_buffer) {
    osMutexAcquire(UserInputScaleHandle, osWaitForever);
    uint16_t vertical_scale = protected_vertical_scale_state;               // 100 - 3300
    uint16_t horizontal_scale_state = *current_horizontal_state;
    if(*current_horizontal_state != protected_horizontal_scale_state){
        horizontal_scale_state = protected_horizontal_scale_state;          // 1 - 11           NOTE -- the horizontal scale is stored as a state NOT A VALUE
        *current_horizontal_state = protected_horizontal_scale_state;
        set_adc_clock_prescalar(horizontal_scale_state); //changes the pre-scalar based on horizontal_scale 
    }
    uint16_t trigger_level = protected_trigger_height;             		    // 0 - 3300
    uint16_t trigger_direction = protected_trigger_state;             		// 1 - up, 0 - down
    osMutexRelease(UserInputScaleHandle);


    // Only 120 pixels can be shown on the screen at once...
    // my ADC DMA buffer can hold 256...
    // the extra DMA space is so that the trigger can "shift"...
    // where from the DMA buffer the data is being pulled from.
    int trigger_offset = 0;
    for(int i = 0; i < 135; i++){   // will time out and just draw the screen if no trigger is detected
        if(trigger_direction && (adcBuffer[i] <= trigger_level && adcBuffer[i+1] > trigger_level)){             //checks smaller than, bigger than, rising
            break;
        } else if(trigger_direction == 0 && (adcBuffer[i] > trigger_level && adcBuffer[i+1] <= trigger_level)){
            break;
        }
        trigger_offset++;
    }
    for(int i = trigger_offset; i < SCREEN_COLUMNS + trigger_offset; i++) {
        int pixels_up = adcBuffer[i] * SCREEN_ROWS * 3300 / 4096 / vertical_scale;
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
    LL_ADC_REG_StartConversion(ADC1);
}

void set_adc_clock_prescalar(uint16_t horizontal_scale_state) {
    //the first five horizontal scales are controlled via ADC pre-scalar aka MODE 1, where-as everything after 1000 u.s. or pre-scalar 10 must be done in software aka MODE 2

    LL_ADC_Disable(ADC1);
    while (LL_ADC_IsEnabled(ADC1)) {}
    switch (horizontal_scale_state) {
        case 0:
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);
            break;
        case 1: 
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV2);
            break;
        case 2:
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV4);
            break;
        case 3: 
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV6);
            break;
        case 4:
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV8);
            break;
        case 5:
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV10);
            break;
        default:
            LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);
            break;
    }
    LL_ADC_Enable(ADC1);
}

void fill_mode2_buffer(uint16_t * horizontal_scale_state, uint16_t * adcBuffer, uint16_t * adcMode2Buffer) {
    for(int i = 0; i < 256; i++) {
    	LL_ADC_REG_StartConversion(ADC1);
    	osDelay(1);
    	adcMode2Buffer[i] = adcBuffer[1];
    }
} 
