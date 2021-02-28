/*!
 * @file  it.c
 * @brief
 *
 */

#include "main.h"

extern TIM_HandleTypeDef timer2Handle;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer2Handle);
}
