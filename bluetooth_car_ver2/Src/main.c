/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum state_t {
 IDLE_S,
 TRIGGERING_S,
 WAITING_FOR_ECHO_START_S,
 WAITING_FOR_ECHO_STOP_S,
 TRIG_NOT_WENT_LOW_S,
 ECHO_TIMEOUT_S,
 ECHO_NOT_WENT_LOW_S,
 READING_DATA_S,
 ERROR_S
} state_t;

volatile state_t state = IDLE_S;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char str1[100] = "NOT STR\n";
unsigned char str2[100] = "STR\n";


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
uint8_t byte;
/* USER CODE BEGIN PFP */
volatile uint32_t echo_start;
volatile uint32_t echo_finish;
volatile uint32_t measured_time;

uint32_t getUs(void) {

	uint32_t usTicks = HAL_RCC_GetSysClockFreq() / 1000000;
	register uint32_t ms, cycle_cnt;
	do {
		ms = HAL_GetTick();
		cycle_cnt = SysTick->VAL;
	} while (ms != HAL_GetTick());
		return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
		}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == ECHOI_Pin )
 {
  switch (state) {
  case WAITING_FOR_ECHO_START_S: {
   echo_start =  getUs();
   state = WAITING_FOR_ECHO_STOP_S;
   break;
  }
  case WAITING_FOR_ECHO_STOP_S: {
   echo_finish = getUs();
   measured_time = echo_finish - echo_start;
   state = READING_DATA_S;
   break;
  }
  default:
   state = ERROR_S;
  }
 }
}

void reset_all(){
	  HAL_GPIO_WritePin(GPIOD, UP_1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, UP_2_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, GPIO_PIN_RESET);
}

void turn_right(){
	reset_all();
	HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
	HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
	HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
}

void go_up(){
	  reset_all();
		  HAL_UART_Transmit(&huart2, str2, strlen((char*)str2), 100);
		  HAL_GPIO_TogglePin(GPIOD, UP_1_Pin);
		  HAL_GPIO_TogglePin(GPIOD, UP_2_Pin);
}

void go_down(){
	  reset_all();
	  HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
	  HAL_GPIO_TogglePin(GPIOD, DOWN_1_Pin);
	  HAL_GPIO_TogglePin(GPIOD, DOWN_2_Pin);
}

uint8_t byte_2;

volatile HAL_StatusTypeDef r;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
	  if (byte == '1'){
		  reset_all();
		  HAL_UART_Transmit(&huart2, str2, strlen((char*)str2), 100);
		  HAL_GPIO_TogglePin(GPIOD, UP_1_Pin);
		  HAL_GPIO_TogglePin(GPIOD, UP_2_Pin);
	    str2[99] = '\0';
	  }
	  if (byte == '2'){
		  reset_all();
		HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
//		  HAL_GPIO_TogglePin(GPIOD, DOWN_1_Pin);
//		  HAL_GPIO_TogglePin(GPIOD, DOWN_2_Pin);
		str1[99] = '\0';
	  }

	  //left
	  if (byte == '3'){
		  	reset_all();
	  		HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
//			HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);
			HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
	  		str1[99] = '\0';
	  	  }

	  //right
	  if (byte == '4'){
		  	reset_all();
	  		HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
			HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
//			HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
	  		str1[99] = '\0';
	  	  }
  }
//  reset_all();
  HAL_UART_Receive_IT(&huart2, &byte, 1);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	extern void initialise_monitor_handles(void);
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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UART_Receive_IT(&huart2, &byte, 1);
  while (1)
  {
	  	  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
	  	   HAL_Delay(16);
	  	   HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

	  	   state = WAITING_FOR_ECHO_START_S;

	  	   while( state == WAITING_FOR_ECHO_START_S && state != ERROR_S )
	  	   {}
	  	   if ( state == ERROR_S )
	  	   {
	  	    printf("Unexpected error while waiting for ECHO to start.\n");
	  	    continue;
	  	   }
	  	   while( state == WAITING_FOR_ECHO_STOP_S && state != ERROR_S )
	  	   {}
	  	   if ( state == ERROR_S )
	  	   {
	  	    printf("Unexpected error while waiting for ECHO to finish.\n");
	  	    continue;
	  	   }

	  	   uint32_t distance = measured_time/58;

	  	   // 60000 us = 1000 sm       60 us = 1 sm
	  	   if (measured_time <= 60000){
	  		   printf("Time: %lu us, distance: %lu cm\n",
	  			 measured_time,
	  			 distance);
	  	   } else {
	  		   printf("ERROR \n");
	  	   }

	  	   if (distance < 30){
	  		   go_down();
	  		   HAL_Delay(200);
	  		   turn_right();
	  		   HAL_Delay(250);
	  		   reset_all();
	  		   HAL_Delay(50);
//	  		   go_up();
	  	   }

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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
