/*!
 * @file  main.c
 * @brief
 *
 */

#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(uint8_t clockFreq);
void Usart2_Init(void);
void ErrorHandler(void);
void CAN1_Init(void);
void CAN1_FilterConfig(void);
void CAN1_Tx(void);
void CAN1_Rx(void);

UART_HandleTypeDef usart2Handle;
CAN_HandleTypeDef can1Handle;

int main(void)
{
	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);
	Usart2_Init();

	CAN1_Init();
	CAN1_FilterConfig();
	if(HAL_CAN_Start(&can1Handle) != HAL_OK)
	{
		ErrorHandler();
	}
	CAN1_Tx();
	CAN1_Rx();

	while(1);

	return 0;
}

void SystemClockConfig(uint8_t clockFreq)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;

	uint32_t fLatency = 0;

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	oscInit.HSIState = RCC_HSI_ON;
	oscInit.HSICalibrationValue = 16;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clockFreq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			oscInit.PLL.PLLM = 16;
			oscInit.PLL.PLLN = 100;
			oscInit.PLL.PLLP = 2;
			oscInit.PLL.PLLQ = 2;
			// oscInit.PLL.PLLR = 2;

			clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
					            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
			clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

			fLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}

		case SYS_CLOCK_FREQ_84_MHZ:
		{
			oscInit.PLL.PLLM = 16;
			oscInit.PLL.PLLN = 168;
			oscInit.PLL.PLLP = 2;
			oscInit.PLL.PLLQ = 2;
			// oscInit.PLL.PLLR = 2;

			clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
					            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clkInit.APB1CLKDivider = RCC_HCLK_DIV2;
			clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

			fLatency = FLASH_ACR_LATENCY_2WS;
			break;
		}

		case SYS_CLOCK_FREQ_120_MHZ:
		{
			oscInit.PLL.PLLM = 16;
			oscInit.PLL.PLLN = 240;
			oscInit.PLL.PLLP = 2;
			oscInit.PLL.PLLQ = 2;
			// oscInit.PLL.PLLR = 2;
			clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
					            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clkInit.APB1CLKDivider = RCC_HCLK_DIV4;
			clkInit.APB2CLKDivider = RCC_HCLK_DIV2;

			fLatency = FLASH_ACR_LATENCY_3WS;
			break;
		}

		default:
		{
			break;
		}
	}

	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK)
	{
		ErrorHandler();
	}

	if(HAL_RCC_ClockConfig(&clkInit, fLatency) != HAL_OK)
	{
		ErrorHandler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
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

void CAN1_Init(void)
{
	can1Handle.Instance = CAN1;
	can1Handle.Init.Mode = CAN_MODE_LOOPBACK;
	can1Handle.Init.AutoBusOff = DISABLE;
	can1Handle.Init.AutoRetransmission = ENABLE;
	can1Handle.Init.AutoWakeUp = DISABLE;
	can1Handle.Init.ReceiveFifoLocked = DISABLE;
	can1Handle.Init.TimeTriggeredMode = DISABLE;
	can1Handle.Init.TransmitFifoPriority = DISABLE;

	// Settings related to CAN bit timings (500 kbps)
	can1Handle.Init.Prescaler = 5;
	can1Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	can1Handle.Init.TimeSeg1 = CAN_BS1_8TQ;
	can1Handle.Init.TimeSeg2 = CAN_BS2_1TQ;

	if(HAL_CAN_Init(&can1Handle) != HAL_OK)
	{
		ErrorHandler();
	}
}

void CAN1_FilterConfig(void)
{
	CAN_FilterTypeDef filterInit;

	filterInit.FilterActivation = ENABLE;
	filterInit.FilterBank = 0;
	filterInit.FilterFIFOAssignment = CAN_RX_FIFO0;
	filterInit.FilterIdHigh = 0x0000;
	filterInit.FilterIdLow = 0x0000;
	filterInit.FilterMaskIdHigh = 0x0000;
	filterInit.FilterMaskIdLow = 0x0000;
	filterInit.FilterMode = CAN_FILTERMODE_IDMASK;
	filterInit.FilterScale = CAN_FILTERSCALE_32BIT;

	if(HAL_CAN_ConfigFilter(&can1Handle, &filterInit) != HAL_OK)
	{
		ErrorHandler();
	}
}

void CAN1_Tx(void)
{
	char msg[50];
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	uint8_t message[5] = {'H', 'e', 'l', 'l', 'o'};

	txHeader.DLC = 5;
	txHeader.StdId = 0x65D;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;

	if(HAL_CAN_AddTxMessage(&can1Handle, &txHeader, message, &txMailbox) != HAL_OK)
	{
		ErrorHandler();
	}

	while(HAL_CAN_IsTxMessagePending(&can1Handle, txMailbox));

	sprintf(msg, "Message transmitted\r\n");
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void CAN1_Rx(void)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t receivedMessage[5];
	char msg[50];

	// Wait for at least one message in to the Rx FIFO 0
	while(!HAL_CAN_GetRxFifoFillLevel(&can1Handle, CAN_RX_FIFO0));

	if(HAL_CAN_GetRxMessage(&can1Handle, CAN_RX_FIFO0, &rxHeader, receivedMessage) != HAL_OK)
	{
		ErrorHandler();
	}

	sprintf(msg, "Message received: %s\r\n", receivedMessage);
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}
