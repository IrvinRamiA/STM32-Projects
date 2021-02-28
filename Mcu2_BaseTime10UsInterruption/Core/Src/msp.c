/*!
 * @file  msp.c
 * @brief
 *
 */

#include "main.h"

void HAL_MspInit(void)
{
	// LOW LEVEL PROCESSOR SPECIFIC INITS

	// 1. Set up the priority grouping of the ARM Cortex MX Processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the ARM Cortex MX Processor
	SCB->SHCSR |= 0x7 << 16;	// Usage Fault, Memory Fault and Bus Fault System Exception

	// 3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *timHandle)
{
	// 1. Enable the clock for the TIM6 peripheral.
	__HAL_RCC_TIM6_CLK_ENABLE();

	// 2. Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	// 3. Setup the priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
}
