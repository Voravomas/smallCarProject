/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
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
#define LOOP_FREQ (SystemCoreClock/4000000)
#define SystemCoreClockInMHz (SystemCoreClock/1000000)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char str1[100] = "NOT STR\n";
unsigned char str2[100] = "STR\n";
unsigned volatile char meters_sec[100];
volatile uint32_t measured_time;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
uint8_t byte;
UART_HandleTypeDef huart2;

unsigned char b_chars[6];
/* USER CODE BEGIN PFP */
volatile uint32_t echo_start;
volatile uint32_t echo_finish;
volatile uint32_t measured_time;
volatile uint32_t num_div;
unsigned volatile char meters_sec[100];

volatile int *arr_ccr[] = {&TIM1->CCR1, &TIM1->CCR2};



void makeBalance(int side) {
	if (side == 1) {
		//going right
		*arr_ccr[0] = 4;
		*arr_ccr[1] = 0;

	} else {
		// going left
		*arr_ccr[0] = 0;
		*arr_ccr[1] = 4;
			//go
		}
	}

void line_mode(){

	*arr_ccr[0] = 4;
	*arr_ccr[1] = 4;
	go_up();

	// line is going left
	while (HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0) {
		makeBalance(0);
	}
	// car is going right
	while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0) {
		makeBalance(1);
	}


}

void line_mode_2(){
	int speed = 6;
	while (byte != 'N'){


		while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 1){
			reset_all();
//			go_up();
	//		HAL_Delay(1000);
			*arr_ccr[0] = 0;
			*arr_ccr[1] = 0;

		}
			while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0){
				reset_all();

				*arr_ccr[0] = speed;
				*arr_ccr[1] = speed;

				HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
				HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
			}

			while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 1){
				reset_all();
				*arr_ccr[0] = speed;
				*arr_ccr[1] = speed;

				HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
				HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);

			}

		while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0){
				reset_all();
				HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
				HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
				*arr_ccr[0] = speed;
				*arr_ccr[1] = speed;
		}
			}
}

void line_mode_3(){
	int speed = 6;
		while (byte != 'N'){


			// STOP
			while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 1 &&
					HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 1 &&
					HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 1){
				reset_all();
	//			go_up();
		//		HAL_Delay(1000);
				*arr_ccr[0] = 0;
				*arr_ccr[1] = 0;

			}

			// LEFT
				while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 0){
					reset_all();

					*arr_ccr[0] = 3;
					*arr_ccr[1] = speed;

					HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
					HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
				}

				// COOL-LEFT
					while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 1){
						reset_all();

						*arr_ccr[0] = 10;
						*arr_ccr[1] = 3;

						HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
						HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
					}

				// RIGHT
				while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 0){
					reset_all();
					*arr_ccr[0] = speed;
					*arr_ccr[1] = 3;

					HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
					HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);

				}

				// COOL-RIGHT
				while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0 && HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 1 && HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 1){
					reset_all();
					*arr_ccr[0] = 3;
					*arr_ccr[1] = 10;

					HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
					HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);
				}

			// UP
			while (HAL_GPIO_ReadPin(GPIOC, RIGHT_LINE_Pin) == 0 &&
					HAL_GPIO_ReadPin(GPIOC, LEFT_LINE_Pin) == 0 &&
					HAL_GPIO_ReadPin(GPIOC, MID_LINE_Pin) == 1){
					reset_all();
					go_up();
					*arr_ccr[0] = speed;
					*arr_ccr[1] = speed;
			}
				}
}


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
//	*arr_ccr[0] = 0;
//	*arr_ccr[1] = 0;
}

void turn_right(){
	reset_all();
//	HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
	HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
	HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
}

void go_up(){
	  reset_all();
//		  HAL_UART_Transmit(&huart2, str2, strlen((char*)str2), 100);
//		  HAL_GPIO_TogglePin(GPIOD, UP_1_Pin);
//		  HAL_GPIO_TogglePin(GPIOD, UP_2_Pin);
			HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
			HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
}

void go_down(){
	  reset_all();
//	  HAL_UART_Transmit(&huart2, str1, strlen((char*)str1), 100);
//	  HAL_GPIO_TogglePin(GPIOD, DOWN_1_Pin);
//	  HAL_GPIO_TogglePin(GPIOD, DOWN_2_Pin);
		HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
		HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);
}

uint8_t byte_2;






volatile HAL_StatusTypeDef r;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {

	  char state;
	  if (byte == 'A'){
		  reset_all();

		  HAL_GPIO_TogglePin(GPIOD, UP_1_Pin);
		  HAL_GPIO_TogglePin(GPIOD, UP_2_Pin);

	    str2[99] = '\0';
	  }
	  if (byte == 'B'){
		  reset_all();
		  HAL_GPIO_TogglePin(GPIOD, DOWN_1_Pin);
		  HAL_GPIO_TogglePin(GPIOD, DOWN_2_Pin);
		str1[99] = '\0';
	  }

	  //left
	  if (byte == 'D'){
		  	reset_all();
//			HAL_GPIO_WritePin(GPIOD, DOWN_2_Pin, 1);
			HAL_GPIO_WritePin(GPIOD, UP_2_Pin, 1);
	  		str1[99] = '\0';
	  	  }

	  //right
	  if (byte == 'C'){
		  	reset_all();
			HAL_GPIO_WritePin(GPIOD, UP_1_Pin, 1);
//			HAL_GPIO_WritePin(GPIOD, DOWN_1_Pin, 1);
	  		str1[99] = '\0';
	  	  }

	  if (byte == '0'){
//		  *arr_ccr[0] = 0;
//		  *arr_ccr[1] = 0;
		  reset_all();
	  }

	  if (byte == '3'){
	  	  		  *arr_ccr[0] = 3;
	  	  		  *arr_ccr[1] = 3;
	  	  	  }
	  if (byte == '4'){
	  	  		  *arr_ccr[0] = 4;
	  	  		  *arr_ccr[1] = 4;
	  	  	  }
	  if (byte == '5'){
	  	  		  *arr_ccr[0] = 5;
	  	  		  *arr_ccr[1] = 5;
	  	  	  }
	  if (byte == '6'){
	  	  		  *arr_ccr[0] = 6;
	  	  		  *arr_ccr[1] = 6;
	  	  	  }
	  if (byte == '7'){
	  	  		  *arr_ccr[0] = 7;
	  	  		  *arr_ccr[1] = 7;
	  	  	  }
	  if (byte == '8'){
	  	  		  *arr_ccr[0] = 8;
	  	  		  *arr_ccr[1] = 8;
	  	  	  }
	  if (byte == '9'){
		  	  		  *arr_ccr[0] = 9;
		  	  		  *arr_ccr[1] = 9;
		  	  	  }
  }

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
	  MX_TIM1_Init();
	  /* USER CODE BEGIN 2 */

	  /* USER CODE END 2 */

	  /* Infinite loop */
	  /* USER CODE BEGIN WHILE */
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	//  for (int i = 0; i < 4; i++) {
	//  	*arr_ccr[i] = 0;
	//  }


	//  HAL_UART_Receive_IT(&huart2, &byte, 1);
	  	HAL_UART_Receive_IT(&huart2, &b_chars, 1);

	  int num_of_dist = 8;
	  int distance_counter = 0;
	  int max_dist = 0;
	  int min_dist = 500000;
	  int all_distance[num_of_dist];
	  int average = 0;


	  uint32_t useconds = SystemCoreClockInMHz * 1000000;

	  uint32_t num_div = 5;
	  while (1)
	  {

	//	  LINE MODE ON
//		  	  byte = 'L';
		  	   if (byte == 'L'){
		  		   line_mode_3();
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
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
