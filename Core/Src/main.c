/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "aes_cmac.h"
#include "boot_jump.h"
#include "uds_port.h"
#include "uds_service.h"
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
uint8_t key[16] = {0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};  // Example key (should be securely stored and managed in production)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define LED_PIN GPIO_PIN_0
#define LED_PORT GPIOC

// 第一步：修正裸机LED初始化（仅操作GPIO，不干扰HAL）
void BareMetal_LED_Init(void) {
    // 1. 启用GPIOC时钟（裸机方式，仅操作必要寄存器）
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // 2. 配置PC0为推挽输出（10MHz，裸机最简配置）
    LED_PORT->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    LED_PORT->CRL |= GPIO_CRL_MODE0_0;
    // 3. 点亮LED：明确表示进入APP的裸机阶段
    LED_PORT->ODR |= LED_PIN;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

    SystemClock_Config();
    /************************ 3. 重映射向量表（时钟稳定后） ************************/
    SCB->VTOR = 0x800C000; // 向量表重映射到APP地址
    __DSB();
    __ISB();

    /************************ 4. 关闭全局中断（初始化HAL前） ************************/
    __disable_irq();

    /************************ 5. 初始化HAL库（此时时钟/SysTick已就绪） ************************/
    HAL_Init();

    /************************ 6. 开启全局中断+后续初始化 ************************/
    __enable_irq();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_CAN_Init();
    uds_init();
    uart_print_hex(key, 16, "enter app: ");
    MX_IWDG_Init();

    /************************ 7. 业务逻辑 ************************/
    while (1)
    {
    static uint32_t cnt_1ms;
    static uint32_t led_cnt;
    if(HAL_GetTick() >= cnt_1ms + 1)
    {
        cnt_1ms = HAL_GetTick();
        uds_1ms_task();
        // LED flashing
        if(led_cnt++ > 250)
        {
          HAL_IWDG_Refresh(&hiwdg);  // Watch dog 500ms timeout
        	led_cnt = 0;
        	 HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // LED flashing
        }
    }
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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

// 第二步：重写HAL_InitTick（适配36MHz时钟，必须！）
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    // 36MHz系统时钟 → SysTick重载值=36000（1ms中断）
    if (HAL_SYSTICK_Config(SystemCoreClock / 1000) != HAL_OK)
    {
        return HAL_ERROR;
    }
    // 设置SysTick优先级（HAL库默认）
    if (TickPriority < (1UL << __NVIC_PRIO_BITS))
    {
        HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
        return HAL_OK;
    }
    return HAL_ERROR;
}