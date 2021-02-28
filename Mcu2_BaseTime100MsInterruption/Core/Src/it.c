/*!
 * @file  it.c
 * @brief
 *
 */

#include "main.h"

extern TIM_HandleTypeDef tim6Handle;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim6Handle);
}
