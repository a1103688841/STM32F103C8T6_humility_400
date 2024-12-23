/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
extern machine_state_t machine_state;
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

/* USER CODE END Variables */
osThreadId          defaultTaskHandle;
uint32_t            defaultTaskBuffer[128];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId          myTask02_buttonHandle;
uint32_t            myTask02_buttonBuffer[128];
osStaticThreadDef_t myTask02_buttonControlBlock;
osThreadId          myTask03_displaHandle;
uint32_t            myTask03_displaBuffer[128];
osStaticThreadDef_t myTask03_displaControlBlock;
osThreadId          myTask04_sensorHandle;
uint32_t            myTask04_sensorBuffer[128];
osStaticThreadDef_t myTask04_sensorControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const* argument);
void StartTask02_button(void const* argument);
void StartTask03_display(void const* argument);
void StartTask04_sensor(void const* argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t  xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
    /* definition and creation of defaultTask */
    osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128, defaultTaskBuffer, &defaultTaskControlBlock);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    /* definition and creation of myTask02_button */
    osThreadStaticDef(myTask02_button, StartTask02_button, osPriorityRealtime, 0, 128, myTask02_buttonBuffer, &myTask02_buttonControlBlock);
    myTask02_buttonHandle = osThreadCreate(osThread(myTask02_button), NULL);

    /* definition and creation of myTask03_displa */
    osThreadStaticDef(myTask03_displa, StartTask03_display, osPriorityHigh, 0, 128, myTask03_displaBuffer, &myTask03_displaControlBlock);
    myTask03_displaHandle = osThreadCreate(osThread(myTask03_displa), NULL);

    /* definition and creation of myTask04_sensor */
    osThreadStaticDef(myTask04_sensor, StartTask04_sensor, osPriorityAboveNormal, 0, 128, myTask04_sensorBuffer, &myTask04_sensorControlBlock);
    myTask04_sensorHandle = osThreadCreate(osThread(myTask04_sensor), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const* argument)
{
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02_button */
/**
 * @brief Function implementing the myTask02_button thread.
 * @param argument: Not used
 * @retval None
 */
#include "../../User/button.h"
void sw0_button_down_callcak()
{
    machine_state.food_types++;
    if (machine_state.food_types > 9)
    {
        machine_state.food_types = 0;
    }
}
void sw0_button_long_callcak()
{
    if (machine_state.display_flag == 1)
    {
        machine_state.display_flag = 0;
    }
    else
    {
        machine_state.display_flag = 1;
    }
}
/* USER CODE END Header_StartTask02_button */
void StartTask02_button(void const* argument)
{
    /* USER CODE BEGIN StartTask02_button */
    Button_Attach_ini();
    /* Infinite loop */
    for (;;)
    {
        Button_Process();
        osDelay(pdMS_TO_TICKS(20));
    }
    /* USER CODE END StartTask02_button */
}

/* USER CODE BEGIN Header_StartTask03_display */
/**
 * @brief Function implementing the myTask03_displa thread.
 * @param argument: Not used
 * @retval None
 */
#include "../../User/tm1637.h"
/* USER CODE END Header_StartTask03_display */
void StartTask03_display(void const* argument)
{
    /* USER CODE BEGIN StartTask03_display */
    tm1637_task_init();
    /* Infinite loop */
    for (;;)
    {
        if (machine_state.humidity_percent > 99)
        {
            machine_state.humidity_percent = 99;
        }
        if (machine_state.food_types > 9)
        {
            machine_state.food_types = 9;
        }
        machine_state.humidity_percent_10 = machine_state.humidity_percent / 10 % 10;
        machine_state.humidity_percent_1  = machine_state.humidity_percent % 10;
        tm1637_task_proceess();
        osDelay(pdMS_TO_TICKS(1000));
    }
    /* USER CODE END StartTask03_display */
}

/* USER CODE BEGIN Header_StartTask04_sensor */
/**
 * @brief Function implementing the myTask04_sensor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04_sensor */
void StartTask04_sensor(void const* argument)
{
    /* USER CODE BEGIN StartTask04_sensor */
    /* Infinite loop */
    for (;;)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 0xffff);
        machine_state.adc1_value                  = HAL_ADC_GetValue(&hadc1);
        machine_state.adc1_voltage                = 3.3 * machine_state.adc1_value / 4096;
        machine_state.humidity_percent = machine_state.adc1_voltage * 30;
        HAL_ADC_Stop(&hadc1);
        osDelay(pdMS_TO_TICKS(1000));
    }
    /* USER CODE END StartTask04_sensor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
