/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define LED_PC13_Pin GPIO_PIN_13
#define LED_PC13_GPIO_Port GPIOC
#define KEY_PA4_Pin GPIO_PIN_4
#define KEY_PA4_GPIO_Port GPIOA
#define KEY_PA5_Pin GPIO_PIN_5
#define KEY_PA5_GPIO_Port GPIOA
#define KEY_PA6_Pin GPIO_PIN_6
#define KEY_PA6_GPIO_Port GPIOA
#define KEY_PA7_Pin GPIO_PIN_7
#define KEY_PA7_GPIO_Port GPIOA
#define KEY_PB0_Pin GPIO_PIN_0
#define KEY_PB0_GPIO_Port GPIOB
#define KEY_PB1_Pin GPIO_PIN_1
#define KEY_PB1_GPIO_Port GPIOB
#define IRReceiver_PB10_Pin GPIO_PIN_10
#define IRReceiver_PB10_GPIO_Port GPIOB
#define KEY_PB12_Pin GPIO_PIN_12
#define KEY_PB12_GPIO_Port GPIOB
#define KEY_PB13_Pin GPIO_PIN_13
#define KEY_PB13_GPIO_Port GPIOB
#define DHT11_PA11_Pin GPIO_PIN_11
#define DHT11_PA11_GPIO_Port GPIOA
#define SCL_PB8_Pin GPIO_PIN_8
#define SCL_PB8_GPIO_Port GPIOB
#define SDA_PB9_Pin GPIO_PIN_9
#define SDA_PB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
