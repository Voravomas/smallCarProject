/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AT_COMAND_Pin GPIO_PIN_5
#define AT_COMAND_GPIO_Port GPIOA
#define SPEED_Pin GPIO_PIN_8
#define SPEED_GPIO_Port GPIOD
#define TRIG_Pin GPIO_PIN_9
#define TRIG_GPIO_Port GPIOD
#define ECHOI_Pin GPIO_PIN_10
#define ECHOI_GPIO_Port GPIOD
#define ECHOI_EXTI_IRQn EXTI15_10_IRQn
#define DOWN_2_Pin GPIO_PIN_12
#define DOWN_2_GPIO_Port GPIOD
#define UP_2_Pin GPIO_PIN_13
#define UP_2_GPIO_Port GPIOD
#define DOWN_1_Pin GPIO_PIN_14
#define DOWN_1_GPIO_Port GPIOD
#define UP_1_Pin GPIO_PIN_15
#define UP_1_GPIO_Port GPIOD
#define RIGHT_LINE_Pin GPIO_PIN_6
#define RIGHT_LINE_GPIO_Port GPIOC
#define LEFT_LINE_Pin GPIO_PIN_7
#define LEFT_LINE_GPIO_Port GPIOC
#define MID_LINE_Pin GPIO_PIN_9
#define MID_LINE_GPIO_Port GPIOC
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
