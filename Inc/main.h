/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define AD9833_CS_Pin GPIO_PIN_11
#define AD9833_CS_GPIO_Port GPIOB
#define AD9833_FSYNC_Pin GPIO_PIN_6
#define AD9833_FSYNC_GPIO_Port GPIOC
#define AD9833_CSC7_Pin GPIO_PIN_7
#define AD9833_CSC7_GPIO_Port GPIOC
#define AD9833_SCK_Pin GPIO_PIN_8
#define AD9833_SCK_GPIO_Port GPIOC
#define AD9833_DATA_Pin GPIO_PIN_9
#define AD9833_DATA_GPIO_Port GPIOC
#define led0_Pin GPIO_PIN_13
#define led0_GPIO_Port GPIOG
#define led1_Pin GPIO_PIN_14
#define led1_GPIO_Port GPIOG
#define led2_Pin GPIO_PIN_15
#define led2_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
