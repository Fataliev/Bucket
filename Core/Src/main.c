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
static uint8_t g_current_adc = 0;
static uint32_t last_usb_test = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "usb_ft232.h"


volatile uint32_t g_adc1_data = 0;
volatile uint32_t g_adc2_data = 0;


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

//Важно проследить что бы функция MX_DMA_Init вызывалась сразу после MX_GPIO_Init (CubeMX нарушает порядок при обновлении, что приводит к сбоям)

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


  // Запускаем TIM3 для генерации прерываний каждую 1 мс
  LL_TIM_EnableIT_UPDATE(TIM3);
  LL_TIM_EnableCounter(TIM3);


   Platform_TickInit();
   SPI_Platform_Init();
   USART_Platform_Init();

   // Инициализация USB
   USB_FT232_Init();
   USB_FT232_SendString("\r\n=== Bucket ADC Test Started ===\r\n");
   last_usb_test = GetTick();


   ADG888_Init();
   AD7799_Init();
  // MeasureFSM_Init();// Внутри уже вызывается AD7799_Init и определяется тип

   // Тестовая проверка ID
   g_adc1_id = AD7799_CheckID(AD7799_CH1);
   g_adc2_id = AD7799_CheckID(AD7799_CH2);

   USB_FT232_SendString("\r\n=== Bucket ADC Test Started ===\r\n");
   USB_FT232_SendString("Press any key to stop...\r\n\r\n");

   last_usb_test = GetTick();
   BSP_LED_V16_Set(1);

   //AD7799_RunFullTest();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { // 1. КОРМЛЕНИЕ СТОРОЖЕВОГО ТАЙМЕРА (Критически важно!)
	  LL_IWDG_ReloadCounter(IWDG);

	    if ((uint32_t)(GetTick() - last_usb_test) >= 1000)
	    {
	        last_usb_test = GetTick();

	        AD7799_Channel_t ch = (g_current_adc == 0) ? AD7799_CH1 : AD7799_CH2;
	        uint8_t ch_num = g_current_adc + 1;

	        // Чтение регистров
	        uint8_t  id     = AD7799_CheckID(ch);
	        uint8_t  status = AD7799_ReadStatus(ch);
	        uint16_t mode   = AD7799_ReadMode(ch);
	        uint16_t config = AD7799_ReadConfig(ch);
	        uint8_t  io     = AD7799_ReadIO(ch);

	        // Чтение всех каналов (каждый запуск изменяет состояние АЦП)
	        uint32_t data_ain1 = AD7799_ReadChannel(ch, AD7799_CONF_BIPOLAR | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(7) | AD7799_CONF_CH_AIN1);
	        uint32_t data_ain2 = AD7799_ReadChannel(ch, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_AIN2);
	        uint32_t data_ain3 = AD7799_ReadChannel(ch, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_AIN3);
	        uint32_t data_temp = AD7799_ReadChannel(ch, AD7799_CONF_BIPOLAR | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_INT_TEMP);

	        // Формируем строку
	        char buf[512];
	        snprintf(buf, sizeof(buf),
	            "\r\n========== ADC CH%d (t=%lu ms) ==========\r\n"
	            "  ID     = 0x%02X  (%s)\r\n"
	            "  Status = 0x%02X  [RDY=%d, ERR=%d, NOREF=%d]\r\n"
	            "  Mode   = 0x%04X  [MD=%d, PSW=%d, FS=0x%X]\r\n"
	            "  Config = 0x%04X  [U/B=%d, G=%d, BUF=%d, CH=%d]\r\n"
	            "  IO     = 0x%02X\r\n"
	            "  -------------------------------------------\r\n"
	            "  CH_AIN1 (G=128, Bipolar) = 0x%06lX  (%lu)\r\n"
	            "  CH_AIN2 (G=1,   Unipolar)= 0x%06lX  (%lu)\r\n"
	            "  CH_AIN3 (G=1,   Unipolar)= 0x%06lX  (%lu)\r\n"
	            "  CH_TEMP (G=1,   Bipolar) = 0x%06lX  (%lu)\r\n"
	            "=============================================\r\n",
	            ch_num,
	            (unsigned long)GetTick(),
	            id,
	            (id == 0x0A) ? "AD7799" : (id == 0x08) ? "AD7798" : "UNKNOWN",
	            status,
	            (status >> 7) & 1,
	            (status >> 6) & 1,
	            (status >> 5) & 1,
	            mode,
	            (mode >> 13) & 7,
	            (mode >> 12) & 1,
	            mode & 0x0F,
	            config,
	            (config >> 12) & 1,
	            (config >> 8) & 7,
	            (config >> 4) & 1,
	            config & 0x07,
	            io,
	            (unsigned long)data_ain1, (unsigned long)data_ain1,
	            (unsigned long)data_ain2, (unsigned long)data_ain2,
	            (unsigned long)data_ain3, (unsigned long)data_ain3,
	            (unsigned long)data_temp, (unsigned long)data_temp
	        );

	        USB_FT232_SendString(buf);

	        // Переключаемся на другой канал
	        g_current_adc = 1 - g_current_adc;
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

