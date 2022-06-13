/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define BUTTON_Pin GPIO_PIN_13
#define BUTTON_GPIO_Port GPIOC
#define PWR_EN_Pin GPIO_PIN_14
#define PWR_EN_GPIO_Port GPIOC
#define MAINS_ZERO_Pin GPIO_PIN_15
#define MAINS_ZERO_GPIO_Port GPIOC
#define TEMPERATURE_8_Pin GPIO_PIN_0
#define TEMPERATURE_8_GPIO_Port GPIOC
#define EVAPORATOR_T_Pin GPIO_PIN_1
#define EVAPORATOR_T_GPIO_Port GPIOC
#define INPUT_V_Pin GPIO_PIN_2
#define INPUT_V_GPIO_Port GPIOC
#define TOTAL_I_Pin GPIO_PIN_3
#define TOTAL_I_GPIO_Port GPIOC
#define TEMPERATURE_1_Pin GPIO_PIN_0
#define TEMPERATURE_1_GPIO_Port GPIOA
#define TEMPERATURE_2_Pin GPIO_PIN_1
#define TEMPERATURE_2_GPIO_Port GPIOA
#define TEMPERATURE_3_Pin GPIO_PIN_4
#define TEMPERATURE_3_GPIO_Port GPIOA
#define BREATHING_Pin GPIO_PIN_5
#define BREATHING_GPIO_Port GPIOA
#define TEMPERATURE_4_Pin GPIO_PIN_6
#define TEMPERATURE_4_GPIO_Port GPIOA
#define TEMPERATURE_5_Pin GPIO_PIN_7
#define TEMPERATURE_5_GPIO_Port GPIOA
#define CONTROL_I_Pin GPIO_PIN_4
#define CONTROL_I_GPIO_Port GPIOC
#define AMBIENT_T_Pin GPIO_PIN_5
#define AMBIENT_T_GPIO_Port GPIOC
#define TEMPERATURE_6_Pin GPIO_PIN_0
#define TEMPERATURE_6_GPIO_Port GPIOB
#define TEMPERATURE_7_Pin GPIO_PIN_1
#define TEMPERATURE_7_GPIO_Port GPIOB
#define ENABLE_7_Pin GPIO_PIN_2
#define ENABLE_7_GPIO_Port GPIOB
#define SPI2_NSS2_Pin GPIO_PIN_13
#define SPI2_NSS2_GPIO_Port GPIOB
#define PWM_5_Pin GPIO_PIN_6
#define PWM_5_GPIO_Port GPIOC
#define PWM_6_Pin GPIO_PIN_7
#define PWM_6_GPIO_Port GPIOC
#define PWM_7_Pin GPIO_PIN_8
#define PWM_7_GPIO_Port GPIOC
#define PWM_8_Pin GPIO_PIN_9
#define PWM_8_GPIO_Port GPIOC
#define PWM_1_Pin GPIO_PIN_8
#define PWM_1_GPIO_Port GPIOA
#define PWM_4_Pin GPIO_PIN_11
#define PWM_4_GPIO_Port GPIOA
#define ENABLE_1_Pin GPIO_PIN_12
#define ENABLE_1_GPIO_Port GPIOA
#define ENABLE_8_Pin GPIO_PIN_15
#define ENABLE_8_GPIO_Port GPIOA
#define ENABLE_2_Pin GPIO_PIN_10
#define ENABLE_2_GPIO_Port GPIOC
#define ENABLE_3_Pin GPIO_PIN_11
#define ENABLE_3_GPIO_Port GPIOC
#define ENABLE_4_Pin GPIO_PIN_12
#define ENABLE_4_GPIO_Port GPIOC
#define ENABLE_5_Pin GPIO_PIN_2
#define ENABLE_5_GPIO_Port GPIOD
#define ENABLE_6_Pin GPIO_PIN_4
#define ENABLE_6_GPIO_Port GPIOB
#define USART1_DE_Pin GPIO_PIN_5
#define USART1_DE_GPIO_Port GPIOB
#define PWM_2_Pin GPIO_PIN_6
#define PWM_2_GPIO_Port GPIOB
#define PWM_3_Pin GPIO_PIN_7
#define PWM_3_GPIO_Port GPIOB
#define AUX_PWM_1_Pin GPIO_PIN_8
#define AUX_PWM_1_GPIO_Port GPIOB
#define AUX_PWM_2_Pin GPIO_PIN_9
#define AUX_PWM_2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
