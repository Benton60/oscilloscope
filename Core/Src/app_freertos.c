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
#include <stdint.h>
#include <stdlib.h>
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

//All these variables are protected under the UserInputScale mnviutex
uint16_t protected_vertical_scale_state = 3300;	    // vertical scale defaults to 3.3
int protected_horizontal_scale_state = 1000;		//horizontal scale defaults to 1000
uint8_t protected_trigger_state = 1;				//trigger defaults to rising
uint16_t protected_trigger_height = 1500; 			//trigger defaults to 2v

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for CharDisplay */
osThreadId_t CharDisplayHandle;
uint32_t MyBufferTask02[ 128 ];
osStaticThreadDef_t MycontrolBlocTask02;
const osThreadAttr_t CharDisplay_attributes = {
  .name = "CharDisplay",
  .stack_mem = &MyBufferTask02[0],
  .stack_size = sizeof(MyBufferTask02),
  .cb_mem = &MycontrolBlocTask02,
  .cb_size = sizeof(MycontrolBlocTask02),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UserInput */
osThreadId_t UserInputHandle;
uint32_t MyBufferTask03[ 128 ];
osStaticThreadDef_t MycontrolBlocTask03;
const osThreadAttr_t UserInput_attributes = {
  .name = "UserInput",
  .stack_mem = &MyBufferTask03[0],
  .stack_size = sizeof(MyBufferTask03),
  .cb_mem = &MycontrolBlocTask03,
  .cb_size = sizeof(MycontrolBlocTask03),
  .priority = (osPriority_t) osPriorityHigh3,
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

  /* creation of CharDisplay */
  CharDisplayHandle = osThreadNew(CharDisplayMain, NULL, &CharDisplay_attributes);

  /* creation of UserInput */
  UserInputHandle = osThreadNew(UserInputMain, NULL, &UserInput_attributes);

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
    uint16_t horizontal_current_state = 5;
    clear_screen();
    for(;;){
    	osDelay(10);
        if(adc_buffer_ready) {
            adc_buffer_ready = 0;
            if(horizontal_current_state > 5) { //states above five need to use an intermediate buffer
                uint16_t adcMode2Buffer[256];
                fill_mode2_buffer(horizontal_current_state, adcBuffer, adcMode2Buffer); 
                transfer_adc_buffer_to_screen_buffer(&horizontal_current_state, adcMode2Buffer, screen_buffer);
            } else { //we can just pass the unmodified adcBuffer to be transferred to the screen buffer for states 0-5
                transfer_adc_buffer_to_screen_buffer(&horizontal_current_state, adcBuffer, screen_buffer);
            }
            draw_SPI_screen();
        }
    }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_CharDisplayMain */
/**
* @brief Function implementing the CharDisplay thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CharDisplayMain */
void CharDisplayMain(void *argument)
{
  /* USER CODE BEGIN CharDisplay */
  char numlist1[4] = {'0', '0', '0', '0'};
  char numlist2[6] = {'0', '0', '0', '0', '0', '0'};
  int16_t varampstate = 0;
  int vartimestate = 0;
  init();
  /* Infinite loop */
  for(;;) {
	osDelay(200);
	command(0x01);
	osDelay(2);
	osMutexAcquire(UserInputScaleHandle, osWaitForever);
	varampstate = protected_vertical_scale_state;
	osMutexRelease(UserInputScaleHandle);
    for(int i = 3; i >= 0; i--) {
    	numlist1[i] = '0' + (varampstate % 10);
    	varampstate = varampstate / 10;
    }
    for(int i = 0; i < 4; i++) {
    	write(numlist1[i]);
    }
    write('m');
    write('V');
    write(0x10);
    write(0x10);
    osMutexAcquire(UserInputScaleHandle, osWaitForever);
    vartimestate = get_time_scale(protected_horizontal_scale_state);
    osMutexRelease(UserInputScaleHandle);
    for(int i = 5; i >= 0; i--) {
    	numlist2[i] = '0' + (vartimestate % 10);
    	vartimestate = vartimestate / 10;
    }
    for(int i = 0; i < 6; i++) {
        write(numlist2[i]);
    }
    write(0xE4);
    write('S');
  }
  /* USER CODE END CharDisplay */
}

/* USER CODE BEGIN Header_UserInputMain */
/**
* @brief Function implementing the UserInput thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UserInputMain */
void UserInputMain(void *argument)
{
  /* USER CODE BEGIN UserInput */
	int16_t ampstate = 0;
	int timestate = 0;
  /* Infinite loop */
  for(;;)
  {
	  osDelay(5);
	  ampstate = set_amplitude_scale(read_amplitude_dial());
	  osMutexAcquire(UserInputScaleHandle, osWaitForever);
	  	  protected_vertical_scale_state = ampstate;
	  osMutexRelease(UserInputScaleHandle);
	  read_time_dial(&timestate);
	  osMutexAcquire(UserInputScaleHandle, osWaitForever);
	  	  protected_horizontal_scale_state = timestate;
	  osMutexRelease(UserInputScaleHandle);
  }
  /* USER CODE END UserInput */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

