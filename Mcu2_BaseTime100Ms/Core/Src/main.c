/*!
 * @file  main.c
 * @brief
 *
 */

#include <string.h>

#include "main.h"

void SystemClockConfig(void);
void ErrorHandler(void);
void Tim6_Init(void);
void Gpio_Init(void);

TIM_HandleTypeDef tim6Handle;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	Gpio_Init();
	Tim6_Init();

	// Let's start the timer
	HAL_TIM_Base_Start(&tim6Handle);

	while(1)
	{
		while(!(TIM6->SR & TIM_SR_UIF));
		TIM6->SR = 0;
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
	}

	return 0;
}

void SystemClockConfig(void)
{

}

void ErrorHandler(void)
{
	while(1);
}

void Tim6_Init(void)
{
	tim6Handle.Instance = TIM6;
	tim6Handle.Init.Prescaler = 24;
	tim6Handle.Init.Period = 64000 - 1;
	if(HAL_TIM_Base_Init(&tim6Handle) != HAL_OK)
	{
		ErrorHandler();
	}
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
