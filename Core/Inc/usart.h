/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define HLPUSART_REC_LEN  256
#define USART2_REC_LEN 256
extern uint8_t  HLPUSART_RX_BUF[HLPUSART_REC_LEN];
extern uint16_t HLPUSART_RX_STA;//接收状�?�标�?
extern uint8_t HLPUSART_NewData;//当前串口中断接收�?1个字节数据的缓存
 
extern uint8_t USART2_RX_BUF[USART2_REC_LEN];
extern uint16_t USART2_RX_STA;
extern uint8_t USART2_NewData;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart);
void nb_printf(char *fmt, ...);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

