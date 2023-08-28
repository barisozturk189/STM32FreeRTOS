/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define     OUTPUT_ON           49 //char to decimal
#define     OUTPUT_OFF          50
#define     OUTPUT_TOGGLE_ON    51
#define     OUTPUT_TOGGLE_OFF   52
#define     OUTPUT_READ_PIN     53
#define     EXIT_API            54

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
osThreadId usermonitorHandle;
osThreadId usermonitoruartHandle;
osThreadId readuserdataHandle;
osThreadId processdataHandle;
osMessageQId myQueue01Handle;
osMessageQId myQueue02Handle;
osTimerId Timer01Handle;
/* USER CODE BEGIN PV */
char monitor[]="\r\n Output on               ->  1 \
	         	\r\n Output off               ->  2 \
		        \r\n Output Toggle on    ->  3 \
		        \r\n Output Toggle off    ->  4 \
	            \r\n Output Read           ->  5 \
		        \r\n Exit App                 ->  6 ";
uint8_t user[1]={0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

void user_monitor(void const * argument);
void user_mon_uart(void const * argument);
void read_user_data(void const * argument);
void process_data(void const * argument);
void PTCallBack(void const * argument);
void vApplicationIdleHook(void);

/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{



	if(!(huart1.Instance->ISR & UART_FLAG_RXNE) ){

		HAL_UART_Receive_IT(&huart1, user,2);


		osSignalSet(usermonitorHandle, 0);

		 osSignalSet(readuserdataHandle, 0);

}


}




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef struct{

	uint8_t num;

	uint8_t command[10];



}APP_COMMAND_t;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */

  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */


  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of Timer01 */
  osTimerDef(Timer01, PTCallBack);
 Timer01Handle = osTimerCreate(osTimer(Timer01), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  HAL_UART_Receive_IT(&huart1,user,2);

  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 10, char*);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

  /* definition and creation of myQueue02 */
  osMessageQDef(myQueue02, 10, sizeof(APP_COMMAND_t*));
  myQueue02Handle = osMessageCreate(osMessageQ(myQueue02), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */


  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */


  /* definition and creation of usermonitor */
  osThreadDef(usermonitor, user_monitor, osPriorityAboveNormal, 0, 128);
  usermonitorHandle = osThreadCreate(osThread(usermonitor), NULL);

  /* definition and creation of usermonitoruart */
  osThreadDef(usermonitoruart, user_mon_uart, osPriorityHigh, 0, 128);
  usermonitoruartHandle = osThreadCreate(osThread(usermonitoruart), NULL);

  /* definition and creation of readuserdata */
  osThreadDef(readuserdata, read_user_data, osPriorityHigh, 0, 128);
  readuserdataHandle = osThreadCreate(osThread(readuserdata), NULL);

  /* definition and creation of processdata */
  osThreadDef(processdata, process_data, osPriorityHigh, 0, 128);
  processdataHandle = osThreadCreate(osThread(processdata), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE9 PE10 PE11 PE12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void readpin(char*outputstatus){



 sprintf(outputstatus,"%d",HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9));

  osMessagePut(myQueue01Handle, (uint32_t)outputstatus, osWaitForever);


}

void toggleonpin(uint32_t delay){


	osTimerStart(Timer01Handle, delay);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);


}
void toggleoffpin(){


	osTimerStop(Timer01Handle);


}
void setpin(void){

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);


}
void resetpin(void){

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);


}
void exitapi(void){

	osThreadTerminate(usermonitoruartHandle);
	osThreadTerminate(usermonitorHandle);
	osThreadTerminate(processdataHandle);
	osThreadTerminate(readuserdataHandle);
}

/* USER CODE END 4 */


/* USER CODE BEGIN Header_user_monitor */
/**
* @brief Function implementing the usermonitor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_user_monitor */
void user_monitor(void const * argument)
{
  /* USER CODE BEGIN user_monitor */
char*msg=monitor;
osMessagePut(myQueue01Handle,(uint32_t)msg, osWaitForever);


  /* Infinite loop */
  for(;;)
  {


	   if(osSignalWait(0,osWaitForever).status==osEventSignal){

		   osMessagePut(myQueue01Handle,(uint32_t)msg, osWaitForever);


	   }
}


  /* USER CODE END user_monitor */
}

/* USER CODE BEGIN Header_user_mon_uart */
/**
* @brief Function implementing the usermonitoruart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_user_mon_uart */
void user_mon_uart(void const * argument)
{
  /* USER CODE BEGIN user_mon_uart */

osEvent event;
   char*rxdata=NULL;
   event=osMessageGet(myQueue01Handle, osWaitForever);
   rxdata=event.value.p;
   HAL_UART_Transmit_IT(&huart1, (uint8_t*)rxdata, strlen(rxdata));

  /* Infinite loop */
  for(;;)
  {
		   event=osMessageGet(myQueue01Handle, osWaitForever);
		   rxdata=event.value.p;
		   HAL_UART_Transmit_IT(&huart1, (uint8_t*)rxdata, strlen(rxdata));




  }
  /* USER CODE END user_mon_uart */
}

/* USER CODE BEGIN Header_read_user_data */
/**
* @brief Function implementing the readuserdata thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_read_user_data */
void read_user_data(void const * argument)
{
  /* USER CODE BEGIN read_user_data */
  /* Infinite loop */

	APP_COMMAND_t *cmd;





  for(;;)
  {
		if(osSignalWait(0, osWaitForever).status==osEventSignal){
	    cmd=(APP_COMMAND_t*)pvPortMalloc(sizeof(APP_COMMAND_t));
        cmd->num=user[0];


       	osMessagePut(myQueue02Handle,(uint32_t)cmd->num , osWaitForever);

      	osSignalSet(processdataHandle, 0);


		}


  }
  /* USER CODE END read_user_data */
}

/* USER CODE BEGIN Header_process_data */
/**
* @brief Function implementing the processdata thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_process_data */
void process_data(void const * argument)
{
	APP_COMMAND_t *cmd;
	char outputstatus[2];
	uint32_t toggletime=pdMS_TO_TICKS(500);
	osEvent event=osMessageGet(myQueue02Handle, osWaitForever);
  for(;;)
  {
	 if(osSignalWait(0, osWaitForever).status==osEventSignal){
		  event=osMessageGet(myQueue02Handle, osWaitForever);
		  cmd=(APP_COMMAND_t*)pvPortMalloc(sizeof(APP_COMMAND_t));
		  cmd->num=(uint8_t)(event.value.v);
	  }

  if((cmd->num)==OUTPUT_ON){
     	 setpin();
    	  }
  else if(cmd->num==OUTPUT_OFF){

 	  resetpin();
	  	  }
  else if(cmd->num==OUTPUT_TOGGLE_ON){

	  toggleonpin(toggletime);

	  	 	  }
  else if(cmd->num==OUTPUT_TOGGLE_OFF){

     toggleoffpin();

	  	  }
  else if(cmd->num==OUTPUT_READ_PIN){
      readpin(outputstatus);

	  	  }
  else if(cmd->num==EXIT_API){

	  exitapi();
  }

  }
}

/* PTCallBack function */
void PTCallBack(void const * argument)
{
  /* USER CODE BEGIN PTCallBack */


	if(user[0]!=OUTPUT_TOGGLE_ON){

		osTimerStop(Timer01Handle);

	}
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);


  /* USER CODE END PTCallBack */
}
void vApplicationIdleHook(void){


	__WFE();

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
