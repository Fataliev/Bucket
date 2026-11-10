/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_iwdg.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_rtc.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

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
#define DS3231S_INT_Pin LL_GPIO_PIN_0
#define DS3231S_INT_GPIO_Port GPIOA
#define USART2_TX_DQ_SWPMI_Pin LL_GPIO_PIN_2
#define USART2_TX_DQ_SWPMI_GPIO_Port GPIOA
#define CS_ADC_2_Pin LL_GPIO_PIN_3
#define CS_ADC_2_GPIO_Port GPIOA
#define CS_ADC_1_Pin LL_GPIO_PIN_4
#define CS_ADC_1_GPIO_Port GPIOA
#define ADC_Heat_Current_Pin LL_GPIO_PIN_0
#define ADC_Heat_Current_GPIO_Port GPIOB
#define CS_PS09_Pin LL_GPIO_PIN_1
#define CS_PS09_GPIO_Port GPIOB
#define DRDY_MAG_Pin LL_GPIO_PIN_2
#define DRDY_MAG_GPIO_Port GPIOB
#define DRDY_MAG_EXTI_IRQn EXTI2_IRQn
#define USB_LED_Pin LL_GPIO_PIN_12
#define USB_LED_GPIO_Port GPIOB
#define USART3_RTS_Pin LL_GPIO_PIN_13
#define USART3_RTS_GPIO_Port GPIOB
#define USART3_CTS_Pin LL_GPIO_PIN_14
#define USART3_CTS_GPIO_Port GPIOB
#define Led_MK_Pin LL_GPIO_PIN_15
#define Led_MK_GPIO_Port GPIOB
#define CS_FLASH_Pin LL_GPIO_PIN_8
#define CS_FLASH_GPIO_Port GPIOA
#define USART1_DTR_USB_DM_Pin LL_GPIO_PIN_11
#define USART1_DTR_USB_DM_GPIO_Port GPIOA
#define USART1_RTR_USB_DP_Pin LL_GPIO_PIN_12
#define USART1_RTR_USB_DP_GPIO_Port GPIOA
#define RESET_USB_Pin LL_GPIO_PIN_15
#define RESET_USB_GPIO_Port GPIOA
#define HEATER_PWM_Pin LL_GPIO_PIN_3
#define HEATER_PWM_GPIO_Port GPIOB
#define LTC4151_SHDN_Pin LL_GPIO_PIN_4
#define LTC4151_SHDN_GPIO_Port GPIOB
#define SW_ADC_Pin LL_GPIO_PIN_5
#define SW_ADC_GPIO_Port GPIOB
#define PULSE_Status_Pin LL_GPIO_PIN_8
#define PULSE_Status_GPIO_Port GPIOB
#define PULSE_mm_Pin LL_GPIO_PIN_9
#define PULSE_mm_GPIO_Port GPIOB
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
