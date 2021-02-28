/*!
 * @file  main.c
 * @brief
 *
 */

#include <string.h>

#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void Usart2_Init(void);
void ErrorHandler(void);
uint8_t ConvertToCapital(uint8_t data);

UART_HandleTypeDef usart2Handle;

char *userData = "The application is running\r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	Usart2_Init();

	uint16_t dataLength = strlen(userData);
	HAL_UART_Transmit(&usart2Handle, (uint8_t *) userData, dataLength, HAL_MAX_DELAY);


	uint8_t receivedData;
	uint8_t dataBuffer[100];
	uint32_t count = 0;
	while(1)
	{
		HAL_UART_Receive(&usart2Handle, &receivedData, 1, HAL_MAX_DELAY);
		if(receivedData == '\r')
		{
			break;
		}
		else
		{
			dataBuffer[count++] = ConvertToCapital(receivedData);
		}
	}

	dataBuffer[count++] = '\r';
	HAL_UART_Transmit(&usart2Handle, dataBuffer, count, HAL_MAX_DELAY);

	while(1);

	return 0;
}

void SystemClockConfig(void)
{
//	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
//
//	/** Configure the main internal regulator output voltage
//	 */
//	__HAL_RCC_PWR_CLK_ENABLE();
//	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//	/** Initializes the RCC Oscillators according to the specified parameters
//	 * in the RCC_OscInitTypeDef structure.
//	 */
//	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	{
//		ErrorHandler();
//	}
//	/** Initializes the CPU, AHB and APB buses clocks
//	 */
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//	{
//		ErrorHandler();
//	}
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
