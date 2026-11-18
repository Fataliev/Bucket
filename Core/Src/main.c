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

#include <stddef.h>
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t g_adc1_id = 0xFF;
volatile uint8_t g_adc2_id = 0xFF;
volatile uint32_t g_adc1_data = 0;
volatile uint32_t g_adc2_data = 0;
volatile uint8_t g_adc1_status = 0xFF;
volatile uint8_t g_adc2_status = 0xFF;

/////////////////


volatile uint8_t g_adc1_mode_h = 0xFF;
volatile uint8_t g_adc1_mode_l = 0xFF;
volatile uint8_t g_adc1_config_h = 0xFF;
volatile uint8_t g_adc1_config_l = 0xFF;
volatile uint8_t g_adc1_io = 0xFF;

// Тестовая функция для последовательного опроса всех регистров
void AD7799_Test(AD7799_Channel_t ch) {
    uint8_t tx_buf[4];
    uint8_t rx_buf[4];

    // 1. Чтение Status Register
    tx_buf[0] = 0x40;
    tx_buf[1] = 0x00;
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(0);
    } else {
        BSP_CS_ADC2_Set(0);
    }
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 2);
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(1);
        g_adc1_status = rx_buf[1];
    } else {
        BSP_CS_ADC2_Set(1);
        g_adc2_status = rx_buf[1];
    }

    // Пауза 1 мс
    for (volatile uint32_t i = 0; i < 72000; i++);

    // 2. Чтение ID Register
    tx_buf[0] = 0x44;
    tx_buf[1] = 0x00;
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(0);
    } else {
        BSP_CS_ADC2_Set(0);
    }
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 2);
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(1);
        g_adc1_id = rx_buf[1];
    } else {
        BSP_CS_ADC2_Set(1);
        g_adc2_id = rx_buf[1];
    }

    // Пауза 1 мс
    for (volatile uint32_t i = 0; i < 72000; i++);

    // 3. Чтение Mode Register (2 байта данных)
    tx_buf[0] = 0x48;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(0);
    } else {
        BSP_CS_ADC2_Set(0);
    }
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(1);
        g_adc1_mode_h = rx_buf[1];
        g_adc1_mode_l = rx_buf[2];
    }

    // Пауза 1 мс
    for (volatile uint32_t i = 0; i < 72000; i++);

    // 4. Чтение Config Register (2 байта данных)
    tx_buf[0] = 0x50;
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(0);
    } else {
        BSP_CS_ADC2_Set(0);
    }
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(1);
        g_adc1_config_h = rx_buf[1];
        g_adc1_config_l = rx_buf[2];
    }

    // Пауза 1 мс
    for (volatile uint32_t i = 0; i < 72000; i++);

    // 5. Чтение I/O Register (1 байт данных)
    tx_buf[0] = 0x68;
    tx_buf[1] = 0x00;
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(0);
    } else {
        BSP_CS_ADC2_Set(0);
    }
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 2);
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(1);
        g_adc1_io = rx_buf[1];
    }
}
\


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
   ADG888_Init();
   AD7799_Init();

   // ПРАВИЛЬНАЯ проверка ID (команда 0xC0)
  // g_adc1_id = AD7799_CheckID(AD7799_CH1);  // Должно быть 0x0A
  // g_adc2_id = AD7799_CheckID(AD7799_CH2);  // Должно быть 0x0A




   uint32_t last_adc_test = GetTick();
   uint32_t last_spi_test = GetTick();
   uint32_t last_test = GetTick();




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { // 1. КОРМЛЕНИЕ СТОРОЖЕВОГО ТАЙМЕРА (Критически важно!)
	  LL_IWDG_ReloadCounter(IWDG);

	  if ((uint32_t)(GetTick() - last_test) >= 2000) {
	        last_test = GetTick();
	        AD7799_Test(AD7799_CH1);
	    }

	    // Принимаем ID = 0x08 (AD7798) или 0x0A (AD7799)
	    if ((g_adc1_id == 0x08 || g_adc1_id == 0x0A) &&
	        (g_adc1_status == 0x08)) {
	        BSP_LED_V16_Set(1);  // АЦП работает
	    } else {
	        BSP_LED_V16_Toggle();
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

