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
#define Length 100  //正弦表中元素个数

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint16_t Sintable[Length]=
  {2048, 2178, 2307, 2436, 2562, 2687, 2809, 2928, 3044, 3155, 3262, 3364, 3461, 3552, 3638, 3716, 3788, 3853, 3911, 3961, 4004, 4038, 4065, 4083, 4094, 4095, 4090, 4075, 4053, 4022, 3983, 3937, 3883, 3822, 3753, 3678, 3596, 3508, 3414, 3314, 3209, 3100, 2986, 2869, 2748, 2625, 2499, 2372, 2243, 2113, 1983, 1853, 1724, 1597, 1471, 1348, 1227, 1110, 996, 887, 782, 682, 588, 500, 418, 343, 274, 213, 159, 113, 74, 43, 21, 6, 0, 2, 13, 31, 58, 92, 135, 185, 243, 308, 380, 458, 544, 635, 732, 834, 941, 1052, 1168, 1287, 1409, 1534, 1660, 1789, 1918, 2048};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO uint8_t AdcConvEnd = 0;  //�?测ADC是否采集完毕
uint32_t Idx;   //定义sin_table中的下标变量
uint16_t DualSineal[Length];    //定义DAC数据缓冲�?????
uint16_t ADC_Value[1000+2]; //储存ADC采集的数�????
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
  /* 用于测试程序是否死机 */
  HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
  HAL_Delay(500);
  HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
  HAL_Delay(500);
  /* 读取正弦波数�????? */
  for(Idx=0;Idx<100;Idx++){
    DualSineal[Idx] = (Sintable[Idx] << 16) + Sintable[Idx];
  }
  HAL_TIM_Base_Start(&htim2);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)DualSineal, Length, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim3);
  HAL_Delay(100);   //等待AD转换结束
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Value, 1000+2); 
  while(!AdcConvEnd);		//等待AD采集完成
	for(X=0;X<1000;X++){
		/* 三次移动平均 */
		for(uint8_t i=0;i<3;i++){
			ADC_Vol+=ADC_Value[X+i];
		}
    ADC_Vol = (double)ADC_Vol*3.3/4095/3;
    printf("%.4f\n",ADC_Vol);	  //用于serialchart波形串口调试          //这一句会让程序死机！！！！！！！！
    }
  // /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* 用于测试程序是否死机 */
      HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
      HAL_Delay(500);
      HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
      HAL_Delay(500);
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
