/*!
 * @file  it.c
 * @brief
 *
 */

#include "main.h"

extern UART_HandleTypeDef usart2Handle;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
