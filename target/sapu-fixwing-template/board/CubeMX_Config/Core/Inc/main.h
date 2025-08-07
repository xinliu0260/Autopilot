/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_SDMMC2_SD_Init(void);
void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_ADC3_Init(void);
void MX_SPI1_Init(void);
void MX_SPI3_Init(void);
void MX_TIM1_Init(void);
void MX_TIM4_Init(void);
void MX_TIM8_Init(void);
void MX_UART4_Init(void);
void MX_UART7_Init(void);
void MX_UART8_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);
void MX_FDCAN1_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AD33V_Pin GPIO_PIN_3
#define AD33V_GPIO_Port GPIOF
#define ADPM_Pin GPIO_PIN_4
#define ADPM_GPIO_Port GPIOF
#define AD54V_Pin GPIO_PIN_5
#define AD54V_GPIO_Port GPIOF
#define RGB_G_Pin GPIO_PIN_1
#define RGB_G_GPIO_Port GPIOA
#define RGB_R_Pin GPIO_PIN_2
#define RGB_R_GPIO_Port GPIOA
#define RGB_B_Pin GPIO_PIN_3
#define RGB_B_GPIO_Port GPIOA
#define SPI1_DR_Pin GPIO_PIN_4
#define SPI1_DR_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_5
#define SPI1_CS_GPIO_Port GPIOC
#define AT45_CS_Pin GPIO_PIN_12
#define AT45_CS_GPIO_Port GPIOA
#define MR_CS_Pin GPIO_PIN_15
#define MR_CS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
