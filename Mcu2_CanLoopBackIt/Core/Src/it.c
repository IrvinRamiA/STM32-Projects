/*!
 * @file  it.c
 * @brief Interrupt handlers
 *
 */

#include "stm32f4xx_hal.h"

extern CAN_HandleTypeDef can1Handle;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&can1Handle);
}

void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&can1Handle);
}

void CAN1_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&can1Handle);
}

void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&can1Handle);
}
