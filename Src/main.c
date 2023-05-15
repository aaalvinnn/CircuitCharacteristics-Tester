/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ad9833.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define Length 200  //正弦表中元素个数
#define AdcLength 1000 //ADC转换数组大小
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint16_t Sintable[Length]=
  {
	1341, 1380, 1420, 1459, 1498, 1536, 1575, 1613, 1651, 1689, 1727, 1764, 1800, 
		1836, 1872, 1907, 1942, 1976, 2009, 2042, 2074, 2105, 2136, 2165, 2194, 2222, 
		2249, 2276, 2301, 2325, 2349, 2371, 2393, 2413, 2432, 2450, 2467, 2483, 2498, 
		2512, 2524, 2535, 2545, 2554, 2562, 2568, 2574, 2578, 2581, 2582, 2582, 2581, 
		2579, 2576, 2571, 2565, 2558, 2550, 2541, 2530, 2518, 2505, 2491, 2475, 2459, 
		2441, 2423, 2403, 2382, 2360, 2337, 2313, 2289, 2263, 2236, 2208, 2180, 2151, 
		2121, 2090, 2058, 2026, 1993, 1959, 1925, 1890, 1854, 1818, 1782, 1745, 1708, 
		1670, 1632, 1594, 1556, 1517, 1478, 1439, 1400, 1361, 1322, 1282, 1243, 1204, 
		1165, 1127, 1088, 1050, 1012, 974, 937, 900, 864, 828, 793, 758, 723, 690, 657, 
		624, 593, 562, 532, 502, 474, 446, 420, 394, 369, 345, 322, 300, 280, 260, 241, 
		224, 207, 192, 177, 164, 153, 142, 132, 124, 117, 111, 107, 103, 101, 100, 100, 
		102, 105, 109, 114, 120, 128, 137, 147, 158, 171, 184, 199, 215, 232, 250, 270, 
		290, 311, 334, 357, 381, 407, 433, 460, 488, 517, 547, 577, 608, 640, 673, 707, 
		741, 775, 810, 846, 882, 919, 956, 993, 1031, 1069, 1107, 1146, 1185, 1224, 1263, 
		1302, 1341
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t AdcConvEnd = 0;  //??????????????????测ADC是否采集完毕
uint32_t Idx;   //定义sin_table中的下标变量
uint16_t DualSineal[Length];    //定义DAC数据缓冲??????????????????????
uint16_t ADC_Value[AdcLength]; //储存ADC采集的数?????????????????????
double ADC_Vol;   //AD采集的真实???
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
  uint16_t X=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* AD9833 发生正弦波 */
  // AD9833_Init(SIN,10000,0);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);

  /* 读取正弦表，转换给DAC */
  for(Idx=0;Idx<Length;Idx++){
    DualSineal[Idx] = (Sintable[Idx] << 16) + Sintable[Idx];
  }
	HAL_TIM_Base_Start(&htim2);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)DualSineal, Length, DAC_ALIGN_12B_R);
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)DualSineal, Length, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim3);
  HAL_Delay(100);   //等待AD转换结束
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Value, AdcLength); 
  while(!AdcConvEnd);		// 等待AD采集完成


  for(X=0;X<AdcLength;X++){
    ADC_Vol = ADC_Value[X] * 3.3 / 4095;
    printf("%.4f\n",ADC_Vol);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim == &htim2){
    static long i=0;
    if(i++ == 100000){
      HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
      i=0;
    }
  }
  else if(htim == &htim3){
    static long j =0;
    if(j++ == 50000) {
      HAL_GPIO_TogglePin(led2_GPIO_Port, led2_Pin);
      j=0;
    }
  }
}
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
