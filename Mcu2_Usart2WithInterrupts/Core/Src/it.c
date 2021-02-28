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

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&usart2Handle);
}
