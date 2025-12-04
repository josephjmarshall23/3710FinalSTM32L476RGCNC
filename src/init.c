#include "pins.h"
#include "systick.h"
#include "stm32l476xx.h"
#include "main.h"
#include "init.h"
#include "Gcode.h"
#include "UART.h"

void system_init()
{
	//Set HSI clock
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);
	RCC->CFGR |= 1;

//	//First, set MSI clock range
//	RCC->CR |= RCC_CR_MSION;
//	while ((RCC->CR & RCC_CR_MSIRDY) == 0); //should only be modified once MSIRDY is high
//	RCC->CR |= RCC_CR_MSIRGSEL;
//	RCC->CR &= ~RCC_CR_MSIRANGE_11; //Clear bits if needed
//	RCC->CR |= RCC_CR_MSIRANGE_11; //Set bits for 48 MHz clock
//	while ((RCC->CR & RCC_CR_MSIRDY) == 0); //Wait for MSI clock to stabilize
//
//	FLASH->ACR &= ~FLASH_ACR_LATENCY; //Set flash wait states to 2
//	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
//
//	RCC->CFGR &= ~RCC_CFGR_SW; //Make sure we use MSI clock
//	RCC->CFGR |= RCC_CFGR_SW_MSI;

	//Set pins defined in pins.h
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable clocks to GPIO A, B, and C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	GPIOA->MODER &= 0xFFD53FFF; //Set PA7 to input (00) and PA8-PA10 as output (01)
	GPIOA->MODER |= 0x00150000;
	GPIOA->OTYPER &= 0xFFFFF8FF; //Set PA8-PA10 as push-pull (0)
	GPIOA->PUPDR &= 0xFFFFBFFF; //Set PA7 as pull-down (10)
	GPIOA->PUPDR |= 0x00008000;

	GPIOB->MODER &= 0xFFDFC57F; //Set PB6 to input (00) and PB3-PB5, PB10 as output (01)
	GPIOB->MODER |= 0x00100540;
	GPIOB->OTYPER &= 0xFFFFFBC7; //Set PB3-PB5 and PB10 as push-pull (0)
	GPIOB->PUPDR &= 0xFFFFDFFF; //Set PB6 to pull-up (01)
	GPIOB->PUPDR |= 0x00001000;

	GPIOC->MODER &= 0xFFFF3FFF; //Set PC7 to input (00)
	GPIOC->PUPDR &= 0xFFFFBFFF; //Set PC7 to pull-down (10)
	GPIOC->PUPDR |= 0x00008000;

	SysTick_Initialize(CLOCK_RATE/INTERRUPT_RATE); //Set SysTick to tick at interrupt rate

	//Enable FPU (this was a nasty bug to find)
	SCB->CPACR |= ((3UL << 20U) | (3UL << 22U));

	lineQueueCount = 0;

	USART_Init(USART2);
	uart_enable();
	startupString(); //Prints a string over serial that tells G-code sender we are running GRBL firmware (we aren't, but pretend)
//	char ch;
//	USART_Read(USART2, (uint8_t*)&ch, 1);
//	if (ch == '\n' || ch == '\r')
//		startupString();
}
