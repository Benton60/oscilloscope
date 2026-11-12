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
#include "trigger_buttons_driver.h"
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
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
  .stack_size = 512 * 4
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
/* Definitions for adcQueue */
osMessageQueueId_t adcQueueHandle;
uint8_t adcQueueBuffer[ 32 * sizeof( uint16_t ) ];
osStaticMessageQDef_t adcQueueControlBlock01;
const osMessageQueueAttr_t adcQueue_attributes = {
  .name = "adcQueue",
  .cb_mem = &adcQueueControlBlock01,
  .cb_size = sizeof(adcQueueControlBlock01),
  .mq_mem = &adcQueueBuffer,
  .mq_size = sizeof(adcQueueBuffer)
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
  /* creation of adcQueue */
  adcQueueHandle = osMessageQueueNew (32, sizeof(uint16_t), &adcQueue_attributes);

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

	uint8_t *screen_buffer = init_spi_screen();
    uint16_t horizontal_current_state = 5;
    clear_screen();
    for(;;){
    	osDelay(10);
    	if(horizontal_current_state != protected_horizontal_scale_state){
    		// 1 - 11           NOTE -- the horizontal scale is stored as a state NOT A VALUE
    	    horizontal_current_state = protected_horizontal_scale_state;
    	    set_adc_clock_prescalar(horizontal_current_state); //changes the pre-scalar based on horizontal_scale
    	}
        if(horizontal_current_state > 5) { //states above five need to use Mode 2
            uint16_t adcMode2Buffer[256];
            fill_mode2_buffer(horizontal_current_state, adcMode2Buffer);
            transfer_adc_buffer_to_screen_buffer(adcMode2Buffer, screen_buffer);
        } else { //we can just pass the unmodified adcBuffer to be transferred to the screen buffer for states 0-5
            transfer_adc_buffer_to_screen_buffer(adcBuffer, screen_buffer);
        }
        draw_SPI_screen();
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
  char numlistamp[4] = {'0', '0', '0', '0'};
  char numlisttime[6] = {'0', '0', '0', '0', '0', '0'};
  char numlisttrig[4] = {'0', '0', '0', '0'};
  int16_t varampstate = 0;
  int vartimestate = 0;
  uint16_t vartrigstate = 0;
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
    	numlistamp[i] = '0' + (varampstate % 10);
    	varampstate = varampstate / 10;
    }
    for(int i = 0; i < 4; i++) {
    	write(numlistamp[i]);
    }
    write('m');
    write('V');
    write(0x10);
    write(0x10);
    osMutexAcquire(UserInputScaleHandle, osWaitForever);
    vartimestate = get_time_scale(protected_horizontal_scale_state);
    osMutexRelease(UserInputScaleHandle);
    for(int i = 5; i >= 0; i--) {
    	numlisttime[i] = '0' + (vartimestate % 10);
    	vartimestate = vartimestate / 10;
    }
    for(int i = 0; i < 6; i++) {
        write(numlisttime[i]);
    }
    write(0xE4);
    write('S');
    osMutexAcquire(UserInputScaleHandle, osWaitForever);
        vartrigstate = protected_trigger_height;
    osMutexRelease(UserInputScaleHandle);
    for(int i = 3; i >= 0; i--) {
       	numlisttrig[i] = '0' + (vartrigstate % 10);
       	vartrigstate = vartrigstate / 10;
    }
    for(int i = 0; i < 4; i++) {
      	write(numlisttrig[i]);
    }
    write('m');
    write('V');
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
	int timestate = 0;
	int triggerchange = 0;
  /* Infinite loop */
  for(;;)
  {
	  osDelay(5);
	  osMutexAcquire(UserInputScaleHandle, osWaitForever);
	  	  protected_vertical_scale_state = set_amplitude_scale(read_amplitude_dial());
	  osMutexRelease(UserInputScaleHandle);
	  read_time_dial(&timestate);
	  osMutexAcquire(UserInputScaleHandle, osWaitForever);
	  	  protected_horizontal_scale_state = timestate;
	  osMutexRelease(UserInputScaleHandle);
	  triggerchange = read_trigger_buttons();
	  osMutexAcquire(UserInputScaleHandle, osWaitForever);
	  	  if(protected_trigger_height != 0)
	  	  {
	  		protected_trigger_height += triggerchange;
	  	  }
	  	  if(triggerchange > 0)
	  	  {
	  		protected_trigger_state = 1;
	  	  }
	  	  else if(triggerchange < 0)
	  	  {
	  		protected_trigger_state = 0;
	  	  }
	  osMutexRelease(UserInputScaleHandle);
  }
  /* USER CODE END UserInput */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

