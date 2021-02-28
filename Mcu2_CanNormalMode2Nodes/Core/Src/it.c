/*!
 * @file  it.c
 * @brief Interrupt handlers
 *
 */

#include "stm32f4xx_hal.h"

extern CAN_HandleTypeDef can1Handle;
extern TIM_HandleTypeDef tim6Handle;

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

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim6Handle);
}

// Change EXTI number for Discovery Board
void EXTI15_10_IRQHandler(void)
{
	HAL_TIM_Base_Start_IT(&tim6Handle);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
