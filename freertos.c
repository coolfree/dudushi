/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "led.h"
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "phone_usb_cdc_rx.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tusb.h"
#include "usb_log.h"
#include "usb_debug.h"
#include "phone_usb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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

/* Definitions for CDC RX Task */

osThreadId_t CDCRxTaskHandle;
const osThreadAttr_t cdcRxTask_attributes =
{
    .name = "CDC_RX",
    .stack_size = 512*4,
    .priority =
        (osPriority_t)
        osPriorityNormal
};

/* USER CODE END Variables */
/* Definitions for AppTask */
osThreadId_t AppTaskHandle;
const osThreadAttr_t AppTask_attributes = {
  .name = "AppTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for USBHostTask */
osThreadId_t USBHostTaskHandle;
const osThreadAttr_t USBHostTask_attributes = {
  .name = "USBHostTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for MTPTask */
osThreadId_t MTPTaskHandle;
const osThreadAttr_t MTPTask_attributes = {
  .name = "MTPTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for CDCTask */
osThreadId_t CDCTaskHandle;
const osThreadAttr_t CDCTask_attributes = {
  .name = "CDCTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myLCDTask */
osThreadId_t myLCDTaskHandle;
const osThreadAttr_t myLCDTask_attributes = {
  .name = "myLCDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myContrytask */
osThreadId_t myContrytaskHandle;
const osThreadAttr_t myContrytask_attributes = {
  .name = "myContrytask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myLED_Task */
osThreadId_t myLED_TaskHandle;
const osThreadAttr_t myLED_Task_attributes = {
  .name = "myLED_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
	/* CDC RX Task */
//void StartTaskCDC_RX(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);
void StartTask06(void *argument);
void LED_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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

  /* Create the thread(s) */
  /* creation of AppTask */
  AppTaskHandle = osThreadNew(StartDefaultTask, NULL, &AppTask_attributes);

  /* creation of USBHostTask */
  USBHostTaskHandle = osThreadNew(StartTask02, NULL, &USBHostTask_attributes);

  /* creation of MTPTask */
  MTPTaskHandle = osThreadNew(StartTask03, NULL, &MTPTask_attributes);

  /* creation of CDCTask */
  CDCTaskHandle = osThreadNew(StartTask04, NULL, &CDCTask_attributes);

  /* creation of myLCDTask */
  myLCDTaskHandle = osThreadNew(StartTask05, NULL, &myLCDTask_attributes);

  /* creation of myContrytask */
  myContrytaskHandle = osThreadNew(StartTask06, NULL, &myContrytask_attributes);

  /* creation of myLED_Task */
  myLED_TaskHandle = osThreadNew(LED_Task, NULL, &myLED_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

/* creation of CDC RX Task */

  CDCRxTaskHandle = osThreadNew(
    PhoneUSB_CDC_RX_Task,
    NULL,
    &cdcRxTask_attributes
  );
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the AppTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the USBHostTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */

    uint8_t key;
    uint8_t last_key = 1U;
	USB_Log("\r\nStartTask02 ENTER\r\n");
/* 开启P4 控制电源	*/
HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_SET);
USB_Log("FS VBUS ON\r\n");


  /* STMPS2151STR 关闭，FSVBUS 由 J17 3-4 直接接 +5V */
//  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_4, GPIO_PIN_RESET);
//  USB_Log("FS VBUS switch OFF\r\n");

/* TinyUSB Host  */

if (!tusb_init())
{
    USB_Log("TinyUSB init FAILED\r\n");
}
else
{
    USB_Log("TinyUSB init OK\r\n");
}

	  /* Infinite loop */
  	  

  for(;;)
  {
        /* Read K1 */
        key = HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin);

        /* K1 pressed: 0 -> 1 */
        if ((last_key == 0U) && (key == 1U))
        {
            USB_Log("\r\n========== K1 PRESSED ==========\r\n");

            USB_FS_RegisterDump();

            USB_Log("================================\r\n");
        }

        last_key = key;
		
        /*----------------------------------------------------------
         * 2. TinyUSB Host task
         *
         * ?? 0ms,??????? FreeRTOS ??
         *----------------------------------------------------------*/
        tuh_task_ext(0, false);

        /*----------------------------------------------------------
         * 3. ??? FreeRTOS ??? USB IRQ ??????
         *----------------------------------------------------------*/
        osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the MTPTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the CDCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myLCDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument)
{
  /* USER CODE BEGIN StartTask05 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
* @brief Function implementing the myContrytask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask06 */
void StartTask06(void *argument)
{
  /* USER CODE BEGIN StartTask06 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask06 */
}

/* USER CODE BEGIN Header_LED_Task */
/**
* @brief Function implementing the myLED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Task */
void LED_Task(void *argument)
{
  /* USER CODE BEGIN LED_Task */
  int brightness = 0;
  int direction = 1;

  /* LED2 初始关闭 */
  LED2_Off(); 

  /* Infinite loop */
  for(;;)
  {
    /* 设置 LED1 当前亮度 */
    LED1_Set((uint8_t)brightness);

    /* 改变亮度 */
    brightness += direction;

    /* 到达最亮 */
    if (brightness >= 100)
    {
      brightness = 100;
      direction = -1;
    }

    /* 到达最暗 */
    else if (brightness <= 0)
    {
      brightness = 0;
      direction = 1;
    }

    /* 每20ms改变一次亮度 */
    osDelay(20);
  }
  /* USER CODE END LED_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

