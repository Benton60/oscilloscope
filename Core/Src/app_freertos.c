/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "char_display_driver.h"
#include "spi_display_driver.h"
#include "voltage_data_points_holder.h"
#include "amplitude_dial_driver.h"
#include "time_dial_driver.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for char_diplay */
osThreadId_t char_diplayHandle;
uint32_t MyBufferTask02[ 128 ];
osStaticThreadDef_t MycontrolBlocTask02;
const osThreadAttr_t char_diplay_attributes = {
  .name = "char_diplay",
  .stack_mem = &MyBufferTask02[0],
  .stack_size = sizeof(MyBufferTask02),
  .cb_mem = &MycontrolBlocTask02,
  .cb_size = sizeof(MycontrolBlocTask02),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UserInputScale */
osMutexId_t UserInputScaleHandle;
osStaticMutexDef_t UserInputScaleControlBlock01;
const osMutexAttr_t UserInputScale_attributes = {
  .name = "UserInputScale",
  .cb_mem = &UserInputScaleControlBlock01,
  .cb_size = sizeof(UserInputScaleControlBlock01),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* creation of UserInputScale */
  UserInputScaleHandle = osMutexNew(&UserInputScale_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of char_diplay */
  char_diplayHandle = osThreadNew(StartTask02, NULL, &char_diplay_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */

	//This code starts the DMA ADC conversions IT SHOULD NOT BE REMOVED EVER
    uint16_t * adcBuffer = (uint16_t *) malloc(256 * sizeof(uint16_t)); //I'm using dynamic allocation because the stack size is only 512 bytes so this buffer fills
                                                                        //the stack by itself. Plus, this is during initialization so it doesn't break any commandments.
	ADC_Start_DMA(adcBuffer);
	//



	uint8_t *screen_buffer = init_spi_screen();
    clear_screen();
    for(;;){
    	osDelay(10);
        if(adc_buffer_ready) {
            adc_buffer_ready = 0;
            transfer_adc_buffer_to_screen_buffer(adcBuffer, screen_buffer);
            draw_SPI_screen();
        }
    }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the char_diplay thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN char_diplay */
	init();
	/*write(0x7E);
	write('C');
	write('O');
	write('O');
	write('L');
	write(0x10);
	write('D');
	write('I');
	write('S');
	write('P');
	write('L');
	write('A');
	write('Y');
	write(0x7F);*/
  /* Infinite loop */
	int16_t ampstate = 0;
	int timestate = 0;
	char numlist1[4] = {'0', '0', '0', '0'};
	char numlist2[6] = {'0', '0', '0', '0', '0', '0'};
  for(;;)
  {
	osDelay(50000);
	command(0x01);
	osDelay(2);
    ampstate = set_amplitude_scale(read_amplitude_dial());
    for(int i = 4; i > 0; i--) {
    	numlist1[i] = '0' + (ampstate % 10);
    	ampstate = ampstate / 10;
    }
    for(int i = 1; i < 5; i++) {
    	write(numlist1[i]);
    }
    write('m');
    write('V');
    write(0x10);
    write(0x10);
    read_time_dial(&timestate);
    int time_value = get_time_scale();
    for(int i = 6; i > 0; i--) {
    	numlist2[i] = '0' + (time_value % 10);
    	time_value = time_value / 10;
    }
    for(int i = 1; i < 7; i++) {
        write(numlist2[i]);
    }
    write(0xE4);
    write('S');
  }
  /* USER CODE END char_diplay */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

