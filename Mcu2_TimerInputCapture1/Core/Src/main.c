/*!
 * @file  main.c
 * @brief
 *
 */

#include "main.h"
#include <string.h>
#include <stdio.h>

void SystemClockConfig(uint8_t clockFreq);
void ErrorHandler(void);
void Gpio_Init(void);
void ErrorHandler(void);
void Tim2_Init(void);
void Lse_Configuration(void);
void Usart2_Init(void);

TIM_HandleTypeDef tim2Handle;
UART_HandleTypeDef usart2Handle;

uint32_t inputCaptures[2] = {0, 0};
uint8_t count = 1;
uint8_t isCaptureDone = FALSE;

int main(void)
{
	uint32_t captureDifference = 0;
	double timer2CntFreq = 0;
	double timer2CntRes = 0;
	double userSignalTimePeriod = 0;
	double userSignalFreq = 0;
	char userMsg[100];

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);
	Gpio_Init();
	Tim2_Init();
	Lse_Configuration();

	while(1)
	{
		if(isCaptureDone)
		{
			if(inputCaptures[1] > inputCaptures[0])
			{
				captureDifference = inputCaptures[1] - inputCaptures[0];
			}
			else
			{
				captureDifference = (0xFFFFFFFF - inputCaptures[0]) + inputCaptures[1];
			}

			timer2CntFreq = (HAL_RCC_GetPCLK1Freq() * 2) / (tim2Handle.Init.Prescaler + 1);
			timer2CntRes = 1 / timer2CntFreq;
			userSignalTimePeriod = captureDifference * timer2CntRes;
			userSignalFreq = 1 / userSignalTimePeriod;

			sprintf(userMsg, "Frequency = %f\r\n", userSignalFreq);
			HAL_UART_Transmit(&usart2Handle, (uint8_t *)userMsg, strlen(userMsg), HAL_MAX_DELAY);

			isCaptureDone = FALSE;
		}
	}

	return 0;
}

void SystemClockConfig(uint8_t clockFreq)
{
	RCC_OscInitTypeDef oscInit;
	RCC_ClkInitTypeDef clkInit;

	uint32_t fLatency = 0;

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	oscInit.HSIState = RCC_HSI_ON;
	oscInit.LSEState = RCC_LSE_ON;
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

void ErrorHandler(void)
{
	while(1);
}

void Gpio_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef ledGpio;
	ledGpio.Pin = GPIO_PIN_12;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &ledGpio);
}

void Tim2_Init(void)
{
	TIM_IC_InitTypeDef tim2InputCapture;

	tim2Handle.Instance = TIM2;
	tim2Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2Handle.Init.Period = 0xFFFFFFFF;
	tim2Handle.Init.Prescaler = 1;

	if(HAL_TIM_IC_Init(&tim2Handle) != HAL_OK)
	{
		ErrorHandler();
	}

	tim2InputCapture.ICFilter = 0;
	tim2InputCapture.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2InputCapture.ICPrescaler = TIM_ICPSC_DIV1;
	tim2InputCapture.ICSelection = TIM_ICSELECTION_DIRECTTI;


	if(HAL_TIM_IC_ConfigChannel(&tim2Handle, &tim2InputCapture, TIM_CHANNEL_1) != HAL_OK)
	{
		ErrorHandler();
	}
}

void Lse_Configuration(void)
{
#if 0
	RCC_OscInitTypeDef oscInit;

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	oscInit.LSEState = RCC_LSE_ON;

	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK)
	{
		ErrorHandler();
	}
#endif

	// LSE Clock Output - PA8
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(!isCaptureDone)
	{
		if(count == 1)
		{
			inputCaptures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else if(count == 2)
		{
			inputCaptures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count = 1;
			isCaptureDone = TRUE;
		}
	}
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
