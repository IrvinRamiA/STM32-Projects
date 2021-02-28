/*!
 * @file  msp.c
 * @brief
 *
 */

#include "stm32f4xx_hal.h"

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
