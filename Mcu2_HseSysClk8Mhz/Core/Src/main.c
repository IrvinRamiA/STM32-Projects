/*!
 * @file   main.c
 * @brief
 * @status It is NOT working yet
 *
 */

#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "main.h"

#define TRUE	1
#define FALSE 	0

void Usart2_Init(void);
void ErrorHandler(void);

UART_HandleTypeDef usart2Handle;

int main(void)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;
	char msg[100];

	HAL_Init();
	Usart2_Init();

	memset(&oscInit, 0, sizeof(oscInit));
	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscInit.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK)
	{
		ErrorHandler();
	}

	clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
	clkInit.APB2CLKDivider = RCC_HCLK_DIV2;


	if(HAL_RCC_ClockConfig(&clkInit, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		ErrorHandler();
	}

	__HAL_RCC_HSI_DISABLE();
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	Usart2_Init();

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "HCLK: %ld\r\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "PCLK1: %ld\r\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	while(1);

	return 0;
}

void Usart2_Init(void)
{
	usart2Handle.Instance = USART2;
	usart2Handle.Init.BaudRate = 9600;
	usart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
	usart2Handle.Init.StopBits = UART_STOPBITS_1;
	usart2Handle.Init.Parity = UART_PARITY_NONE;
	usart2Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	usart2Handle.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&usart2Handle) != HAL_OK)
	{
		ErrorHandler();
	}
}

void ErrorHandler(void)
{
	while(1);
}
