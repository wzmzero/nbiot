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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bc35.h"
#include "ds18b20.h"
#include "stdio.h"
#include "mq7.h"
#include "dht11.h"
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

    HAL_Delay(1000);
	uint16_t  DHT11_BUF[2]={0};
	char display_temp[20] = {0};
    char display_mq[20] = {0};
    float temperature;
    char str_temp[20]={0};
    char str_humi[20]={0};
	HAL_UART_Receive_IT(&huart1, &HLPUSART_NewData, 1);
	HLPUSART_RX_STA = 0;
	HAL_UART_Receive_IT(&huart2, &USART2_NewData, 1);
	USART2_RX_STA = 0;
    while(DS18B20_Init()){
        uart_printf(&huart1,"DS18B20 checked failed!!!\r\n");
        HAL_Delay(500);
    }
    while(DHT11_Init()){
        uart_printf(&huart1,"DHT11 checked failed!!!\r\n");
        HAL_Delay(500);
    }
    float mq;
    Init_MQ7();
	nb_init();
    nb_reg();
    nb_sub();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		DHT11_ReadData(DHT11_BUF);
        sprintf(str_temp,"%d.%d",DHT11_BUF[1]/10,DHT11_BUF[1]%10);
        sprintf(str_humi,"%d.%d",DHT11_BUF[0]/10,DHT11_BUF[0]%10);
//		printf("%s,%s\r\n",str_temp,str_humi);
//       HAL_Delay(2000);
      float crc=DS18B20_Get_Temp();
      if(crc>0.0)temperature = crc;
      sprintf(display_temp,"%.1f",temperature);
      mq=MQ7_GetPPM();
      sprintf(display_mq,"%.2f",mq);
//      printf("%s,%s\r\n",display_temp,display_mq);
      nb_send(str_temp,str_humi,display_mq);
      HAL_Delay(200);
//	 if(HLPUSART_RX_STA&0xC000){
////         uart_printf(&huart2,"%.*s\r\n",HLPUSART_RX_STA&0X0FFF, HLPUSART_RX_BUF);
//		  nb_printf("%.*s\r\n",HLPUSART_RX_STA&0X0FFF, HLPUSART_RX_BUF);
//		  HLPUSART_RX_STA=0;
//		  HAL_Delay(100);
//	  }
//	  if(USART2_RX_STA&0xC000){
//          uart_printf(&huart1,"%.*s\r\n",USART2_RX_STA&0X0FFF, USART2_RX_BUF);
//  		  USART2_RX_STA=0;
//  		  HAL_Delay(100);
//  	  }

      char str_rec=nb_recv();
      if(str_rec=='l'){
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
              printf("off %c\r\n", str_rec);
          }
      if(str_rec=='r'){
              HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
              printf("on %c\r\n", str_rec);
          }
//      HAL_Delay(800);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
