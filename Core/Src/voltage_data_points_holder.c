#include "voltage_data_points_holder.h"
#include "app_freertos.h"
#include "spi_display_driver.h"
#include "main.h"


uint16_t adcBuffer[256];

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

void holder_voltage_points() {
    return; //TODO
}
void * get_adcBuffer() {
    return adcBuffer;
}

void transfer_adc_buffer_to_screen_buffer(void * adcBuffer_null, void * screen_buffer_null) {
    uint16_t * adcBuffer = adcBuffer_null;
    uint8_t * screen_buffer = screen_buffer_null;
    uint16_t vertical_scale = 3500;     // 100 - 3300
    uint16_t horzontal_scale = 100;     // 100 - 100000
    uint16_t trigger_level  = 1000;     // 0 - 3300
    uint16_t trigger_direction = 1;     // 1 - up, 0 - down


    // Only 120 pixels can be shown on the screen at once...
    // my adc DMA buffer can hold 256...
    // the extra DMA space is so that the trigger can "shift"...
    // where from the DMA buffer the data is being pulled from.
    int trigger_offset = 0;
    for(int i = 0; i < 145; i++){
        if(adcBuffer[i] <= trigger_level && adcBuffer[i+1] > trigger_level){
            break;
        }
        trigger_offset++;
    }
    for(int i = trigger_offset; i < SCREEN_COLUMNS + trigger_offset; i++) {
        int pixels_up = adcBuffer[i * horzontal_scale / 100] * SCREEN_ROWS / 4096 * 3300 / vertical_scale;
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

