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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define Length 200  //正弦表中元素个数
#define AdcLength 1000 //ADC转换数组大小
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint16_t Sintable[Length]=
  {2048, 2113, 2177, 2242, 2306, 2370, 2434, 2497, 2560, 2622, 2684, 2745, 2806, 
  2865, 2924, 2982, 3039, 3095, 3150, 3204, 3257, 3309, 3359, 3408, 3456, 3502, 
  3547, 3590, 3632, 3672, 3711, 3747, 3783, 3816, 3848, 3878, 3906, 3932, 3957, 
  3979, 4000, 4018, 4035, 4050, 4062, 4073, 4082, 4088, 4093, 4095, 4095, 4094, 
  4091, 4085, 4078, 4068, 4056, 4043, 4027, 4009, 3990, 3968, 3945, 3919, 3892, 
  3863, 3832, 3800, 3765, 3729, 3691, 3652, 3611, 3569, 3524, 3479, 3432, 3384, 
  3334, 3283, 3231, 3177, 3123, 3067, 3011, 2953, 2895, 2835, 2775, 2715, 2653, 
  2591, 2528, 2465, 2402, 2338, 2274, 2209, 2145, 2080, 2016, 1951, 1887, 1822, 
  1758, 1694, 1631, 1568, 1505, 1443, 1381, 1321, 1261, 1201, 1143, 1085, 1029, 
  973, 919, 865, 813, 762, 712, 664, 617, 572, 527, 485, 444, 405, 367, 331, 296, 
  264, 233, 204, 177, 151, 128, 106, 87, 69, 53, 40, 28, 18, 11, 5, 2, 0, 1, 3, 8, 
  14, 23, 34, 46, 61, 78, 96, 117, 139, 164, 190, 218, 248, 280, 313, 349, 385, 424, 
  464, 506, 549, 594, 640, 688, 737, 787, 839, 892, 946, 1001, 1057, 1114, 1172, 1231, 
  1290, 1351, 1412, 1474, 1536, 1599, 1662, 1726, 1790, 1854, 1919, 1983, 2048
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t AdcConvEnd = 0;  //�????????????????测ADC是否采集完毕
uint32_t Idx;   //定义sin_table中的下标变量
uint16_t DualSineal[Length];    //定义DAC数据缓冲�????????????????????
uint16_t ADC_Value[AdcLength]; //储存ADC采集的数�???????????????????
double ADC_Vol;   //AD采集的真实�??
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


  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);

  /* 读取正弦表，转换给DAC */
  for(Idx=0;Idx<Length;Idx++){
    DualSineal[Idx] = (Sintable[Idx] << 16) + Sintable[Idx];
  }
  HAL_TIM_Base_Start(&htim2);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)DualSineal, Length, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim3);
  HAL_Delay(100);   //等待AD转换结束
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Value, AdcLength); 
  while(!AdcConvEnd);		// 等待AD采集完成

	// for(X=0;X<AdcLength;X++){
  //   ADC_Vol=0;
	// 	/* 三次移动平均 */
	// 	for(uint8_t i=0;i<3;i++){
	// 		ADC_Vol+=ADC_Value[X+i];
	// 	}
  //   ADC_Vol = (double)ADC_Vol*3.3/4095/3;
  //   printf("%.4f\n",ADC_Vol);	  //用于serialchart波形串口调试
  //   }

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
