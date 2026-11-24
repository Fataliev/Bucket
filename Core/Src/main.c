/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "platform_tick.h"
#include "bsp_gpio.h"
#include "periph_spi.h"
#include "periph_usart.h"
#include "adg888.h"
#include "ad7799.h"
#include "measure_fsm.h"
#include "usb_ft232.h"
#include "super_cycle.h"
#include "platform_events.h"


#include <stddef.h>
#include <stdio.h>
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

/* USER CODE BEGIN PV */
volatile uint8_t g_adc1_id = 0xFF;
volatile uint8_t g_adc2_id = 0xFF;
volatile MeasureResult_t g_measure_result; // <-- Теперь тип известен компилятору
volatile uint8_t g_measurement_complete = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  MX_USART3_UART_Init();
 // MX_IWDG_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    /* --- Приоритеты прерываний (ТЗ 2.2.5, SysTick выше периферии!) --- */
    NVIC_SetPriority(SysTick_IRQn,         NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_SetPriority(USART3_IRQn,          NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
    NVIC_SetPriority(SPI1_IRQn,            NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
    NVIC_SetPriority(TIM4_IRQn,            NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
    NVIC_SetPriority(TIM3_IRQn,            NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
    NVIC_SetPriority(USART1_IRQn,          NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 4, 0));
    NVIC_SetPriority(DMA1_Channel4_IRQn,   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_SetPriority(DMA1_Channel7_IRQn,   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_SetPriority(DMA1_Channel2_IRQn,   NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));

    /* --- Слой Platform --- */
    Platform_TickInit();
    Events_Init();
    SPI_Platform_Init();
    USART_Platform_Init();
    USB_FT232_Init();

    USB_FT232_SendString("\r\n=== Bucket boot ===\r\n");
    if (Platform_TickIsRunning() == 0U) {
        for (;;) {
            USB_FT232_SendString("FATAL: SysTick IRQ not running\r\n");
            BSP_LED_V16_Toggle();
            Platform_DelayUs(500000U);
        }
    }
    USB_FT232_SendString("SysTick OK (1 ms)\r\n");

    /* --- Безопасные состояния шины ДО первого обмена --- */
    BSP_CS_ADC1_Set(1);
    BSP_CS_ADC2_Set(1);
    BSP_CS_FLASH_Set(1);
    BSP_CS_PS09_Set(1);
    ADG888_Init();

    /* --- Приложение: AD7799_Init вызывается ОДИН раз, внутри MeasureFSM_Init --- */
    MeasureFSM_Init();
    SuperCycle_Init();

    USB_FT232_Printf("D22 ID=0x%02X OFFSET=0x%06lX FS=0x%06lX\r\n",
        AD7799_ReadID(AD7799_DEV_1),
        (unsigned long)AD7799_ReadOffset(AD7799_DEV_1),
        (unsigned long)AD7799_ReadFullScale(AD7799_DEV_1));
    USB_FT232_Printf("D23 ID=0x%02X OFFSET=0x%06lX FS=0x%06lX\r\n",
        AD7799_ReadID(AD7799_DEV_2),
        (unsigned long)AD7799_ReadOffset(AD7799_DEV_2),
        (unsigned long)AD7799_ReadFullScale(AD7799_DEV_2));

    /* --- Запуск таймеров: UIF сбрасываем, иначе первое прерывание ложное --- */
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_TIM_EnableIT_UPDATE(TIM3);
    LL_TIM_EnableCounter(TIM3);

    LL_TIM_ClearFlag_UPDATE(TIM4);
    LL_TIM_EnableIT_UPDATE(TIM4);
    LL_TIM_EnableCounter(TIM4);

    USB_FT232_SendString("Super-cycle started (TIM4 = 6 s)\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

   while (1)
   {
       // 1. КОРМЛЕНИЕ СТОРОЖЕВОГО ТАЙМЕРА (Критически важно!)
       //LL_IWDG_ReloadCounter(IWDG);

	   SuperCycle_Run();

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
  }
  LL_RCC_LSE_Enable();

   /* Wait till LSE is ready */
  while(LL_RCC_LSE_IsReady() != 1)
  {

  }
  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSE)
  {
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
  }
  LL_RCC_EnableRTC();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
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

