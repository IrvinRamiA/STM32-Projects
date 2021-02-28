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
void TIM6_Init(void);
void GPIO_Init(void);
void LED_ManageOutput(uint8_t ledNumber);
void SendResponse(uint32_t stdId);

UART_HandleTypeDef usart2Handle;
CAN_HandleTypeDef can1Handle;
TIM_HandleTypeDef tim6Handle;
uint8_t ledNumber = 0;
CAN_RxHeaderTypeDef rxHeader;
uint8_t reqCounter = 0;

int main(void)
{
	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_84_MHZ);
	GPIO_Init();
	Usart2_Init();
	TIM6_Init();

	CAN1_Init();
	CAN1_FilterConfig();
    if(HAL_CAN_ActivateNotification(&can1Handle, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
    {
    	ErrorHandler();
    }

    if(HAL_CAN_Start(&can1Handle) != HAL_OK)
	{
		ErrorHandler();
	}
	CAN1_Tx();

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
	can1Handle.Init.Mode = CAN_MODE_NORMAL;
	can1Handle.Init.AutoBusOff = ENABLE;
	can1Handle.Init.AutoRetransmission = ENABLE;
	can1Handle.Init.AutoWakeUp = DISABLE;
	can1Handle.Init.ReceiveFifoLocked = DISABLE;
	can1Handle.Init.TimeTriggeredMode = DISABLE;
	can1Handle.Init.TransmitFifoPriority = DISABLE;

	// Settings related to CAN bit timings (500 kbps)
	can1Handle.Init.Prescaler = 3;
	can1Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	can1Handle.Init.TimeSeg1 = CAN_BS1_11TQ;
	can1Handle.Init.TimeSeg2 = CAN_BS2_2TQ;

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
	filterInit.FilterMaskIdHigh = 0x01C0;
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
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	uint8_t message;

	txHeader.DLC = 1;
	txHeader.StdId = 0x65D;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;

	message = ++ledNumber;

	if(ledNumber == 4)
	{
		ledNumber = 0;
	}

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

	if(HAL_CAN_AddTxMessage(&can1Handle, &txHeader, &message, &txMailbox) != HAL_OK)
	{
		ErrorHandler();
	}
}

void CAN1_Rx(void)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t receivedMessage[5];
	char msg[50];

	if(HAL_CAN_GetRxMessage(&can1Handle, CAN_RX_FIFO0, &rxHeader, receivedMessage) != HAL_OK)
	{
		ErrorHandler();
	}

	sprintf(msg, "Message received: %s\r\n", receivedMessage);
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void TIM6_Init(void)
{
	tim6Handle.Instance = TIM6;
	tim6Handle.Init.Prescaler = 4999;
	tim6Handle.Init.Period = 10000 - 1;

	if(HAL_TIM_Base_Init(&tim6Handle) != HAL_OK)
	{
		ErrorHandler();
	}
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef ledGpio;

	ledGpio.Pin = GPIO_PIN_5;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledGpio);

	ledGpio.Pin = GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_6;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &ledGpio);

	ledGpio.Pin = GPIO_PIN_8;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &ledGpio);

	ledGpio.Pin = GPIO_PIN_13;
	ledGpio.Mode = GPIO_MODE_IT_FALLING;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &ledGpio);

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void LED_ManageOutput(uint8_t ledNumber)
{
	switch(ledNumber)
	{
		case 1:
		{
			HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		}
		case 2:
		{
			HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		}
		case 3:
		{
			HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
		}
		case 4:
		{
			HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_SET);
		}
	}
}

void SendResponse(uint32_t stdId)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	uint8_t response[2] = {0xAB, 0xCD};

	txHeader.DLC = 2;
	txHeader.StdId = stdId;
	txHeader.IDE = CAN_ID_STD;
	txHeader.RTR = CAN_RTR_DATA;

	if(HAL_CAN_AddTxMessage(&can1Handle, &txHeader, response, &txMailbox) != HAL_OK)
	{
		ErrorHandler();
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[50];

	sprintf(msg, "Message transmitted - M0\r\n");
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[50];

	sprintf(msg, "Message transmitted - M1\r\n");
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	char msg[50];

	sprintf(msg, "Message transmitted - M2\r\n");
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t receivedMessage[8];
	char msg[50];

	if(HAL_CAN_GetRxMessage(&can1Handle, CAN_RX_FIFO0, &rxHeader, receivedMessage) != HAL_OK)
	{
		ErrorHandler();
	}

	if(rxHeader.StdId == 0x65D && rxHeader.RTR == 0)
	{
		// This is data from sent by N1 to N2
		LED_ManageOutput(receivedMessage[0]);
		sprintf(msg, "Message Received: #%x\r\n", receivedMessage[0]);
	}
	else if(rxHeader.StdId == 0x651 && rxHeader.RTR == 1)
	{
		// This is a remote frame sent by N1 to N2
		SendResponse(rxHeader.StdId);
		return;
	}
	else if(rxHeader.StdId == 0x651 && rxHeader.RTR == 0)
	{
		// This is a reply (data frame) by N1 to N2
		sprintf(msg, "Reply Received: %#X\r\n", receivedMessage[0] << 8 | receivedMessage[1]);
	}

	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char msg[50];

	sprintf(msg, "CAN Error Detected\r\n");
	HAL_UART_Transmit(&usart2Handle, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;
	uint8_t message;	// No meaning for Data Frame

	if(reqCounter == 4)
	{
		// N1 sending Remote Frame to N2
		txHeader.DLC = 2;	// N1 demanding 2 bytes of reply
		txHeader.StdId = 0x651;
		txHeader.IDE = CAN_ID_STD;
		txHeader.RTR = CAN_RTR_REMOTE;

		if(HAL_CAN_AddTxMessage(&can1Handle, &txHeader, &message, &txMailbox) != HAL_OK)
		{
			ErrorHandler();
		}
		reqCounter = 0;
	}
	else
	{
		CAN1_Tx();
		reqCounter++;
	}
}
