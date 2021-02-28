/*!
 * @file  main.c
 * @brief
 *
 */

#include <string.h>

#include "stm32f4xx_hal.h"
#include "main.h"

#define TRUE	1
#define FALSE 	0

void SystemClockConfig(void);
void Usart2_Init(void);
void ErrorHandler(void);
uint8_t ConvertToCapital(uint8_t data);

UART_HandleTypeDef usart2Handle;
uint8_t dataBuffer[100];
uint8_t receivedData;
uint32_t count = 0;
uint8_t receptionComplete = FALSE;

char *userData = "The application is running!!!\r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	Usart2_Init();

	uint16_t dataLength = strlen(userData);
	HAL_UART_Transmit(&usart2Handle, (uint8_t *) userData, dataLength, HAL_MAX_DELAY);

	while(receptionComplete != TRUE)
	{
		HAL_UART_Receive_IT(&usart2Handle, &receivedData, 1);
	}

	while(1);

	return 0;
}

void SystemClockConfig(void)
{

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

uint8_t ConvertToCapital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data -= ('a' - 'A');
	}

	return data;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(receivedData == '\r')
	{
		receptionComplete = TRUE;
		dataBuffer[count++] = '\r';
		HAL_UART_Transmit(&usart2Handle, dataBuffer, count, HAL_MAX_DELAY);
	}
	else
	{
		dataBuffer[count++] = receivedData;
	}
}
