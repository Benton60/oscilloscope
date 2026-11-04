#include "voltage_data_points_holder.h"
#include "app_freertos.h"
#include "main.h"


void ADC_Start_DMA(void * adcBuffer) {
    LL_ADC_Enable(ADC1);

    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1))
    {
    }

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
 
