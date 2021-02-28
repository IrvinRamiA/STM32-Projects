/*!
 * @file   main.c
 * @brief
 * @status Working properly
 *
 */

#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "main.h"

void Gpio_Init(void);
void Timer2_Init(void);
void Usart2_Init(void);
void ErrorHandler(void);
void SystemClockConfig(uint8_t clockFreq);

UART_HandleTypeDef usart2Handle;
TIM_HandleTypeDef timer2Handle;

int main(void)
{
	uint16_t brightness = 0;

	HAL_Init();
	SystemClockConfig(SYS_CLOCK_FREQ_120_MHZ);
	Gpio_Init();
	Usart2_Init();
	Timer2_Init();

	if(HAL_TIM_PWM_Start(&timer2Handle, TIM_CHANNEL_1) != HAL_OK)
	{
		ErrorHandler();
	}

	while(1)
	{
		while(brightness < timer2Handle.Init.Period)
		{
			brightness += 20;
			__HAL_TIM_SET_COMPARE(&timer2Handle, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
		}

		while(brightness > 0)
		{
			brightness -= 20;
			__HAL_TIM_SET_COMPARE(&timer2Handle, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
		}
	}

	return 0;
}

void Gpio_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledGpio;
	ledGpio.Pin = GPIO_PIN_5;
	ledGpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledGpio);
}

void Timer2_Init(void)
{
	TIM_OC_InitTypeDef tim2PwmConfig;
	timer2Handle.Instance = TIM2;
	timer2Handle.Init.Period = 10000 - 1;
	timer2Handle.Init.Prescaler = 4;
	if(HAL_TIM_PWM_Init(&timer2Handle) != HAL_OK)
	{
		ErrorHandler();
	}

	memset(&tim2PwmConfig, 0, sizeof(tim2PwmConfig));

	tim2PwmConfig.OCMode = TIM_OCMODE_PWM1;
	tim2PwmConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

	tim2PwmConfig.Pulse = 0;
	if(HAL_TIM_PWM_ConfigChannel(&timer2Handle, &tim2PwmConfig, TIM_CHANNEL_1)
			!= HAL_OK)
	{
		ErrorHandler();
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

void ErrorHandler(void)
{
	while(1);
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
