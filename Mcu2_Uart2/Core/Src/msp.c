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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// LOW LEVEL INITS OF THE USART2 PERIPHERAL

	GPIO_InitTypeDef gpioUsart2;

	// 1. Enable the clock for the USART2 and GPIOA peripherals
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Pin muxing configurations
	// PA2 - USART2_TX
	// PA3 - USART2_RX
	gpioUsart2.Pin = GPIO_PIN_2;
	gpioUsart2.Mode = GPIO_MODE_AF_PP;
	gpioUsart2.Pull = GPIO_PULLUP;
	gpioUsart2.Speed = GPIO_SPEED_FREQ_LOW;
	gpioUsart2.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpioUsart2);

	gpioUsart2.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpioUsart2);

	// 3. Enable the IRQ and set up the priority (NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}
