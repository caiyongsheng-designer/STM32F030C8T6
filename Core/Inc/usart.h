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

/* USER CODE BEGIN Private defines */
typedef struct __FILE FILE;
#define USART_REC_MAX_LEN  			200  	//定义最大接收字节数 200
struct UART_RECEIVE{
	   uint8_t UART1_Receive_Flag;
	   uint8_t UART1_Receive_data_len;
     uint8_t * UART1_Receive_data;
     
};
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void MX_USART1_UART_Init(void);
void USART1_IdleCallback(uint8_t *pData,uint16_t len);
struct	UART_RECEIVE Uart1_Receive_data(void);
void Left_Move_Array(uint8_t A[], uint8_t n, uint8_t k);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

