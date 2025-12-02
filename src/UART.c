#include "stm32l476xx.h"
#include "UART.h"


void USART_Init (USART_TypeDef * USARTx) {
	
	//from main
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; 	//enable GPIOA clk
	
	// GPIO Mode: Input(00), Output (01), 
	// AF(10), Analog (11)
	GPIOA->MODER &= 0xFFFFFF0F; //set PA2-3 to inputs (cleared)
	GPIOA->MODER |= 0x000000A0; //set PA2-3 to alternate function
	
	//select alt functions as 2
	GPIOA->AFR[0] |= 0x77<<(2*4);
	
	GPIOA->OSPEEDR |= 0x000000F0; //set PA2 and 3 to high speed
	
	//set pull up on TX output
	GPIOA->PUPDR &= 0xFFFFFF0F; //clear pupdr for PA2-3
	GPIOA->PUPDR |= 0x00000050; //set pull up on PA2-3
	
	//output TX is set to push pull (this is the default)
	
	//enable USART2 clk
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;   //this needs to be changed to USART2
	
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	
	//end from main
	
	
	//Disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	
	//set data length to 8 bits
	USARTx->CR1 &= ~USART_CR1_M;
	
	//select 1 stop bit
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	//no parity
	USARTx->CR1 &= ~USART_CR1_PCE;
	
	//oversampling by 16
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	//Set baud rate to 115200 using 16MHz
	USARTx->BRR = 0x08B; 							
	
	//enable transmission and reception
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	//enable uart
	USARTx->CR1 |= USART_CR1_UE;
	
	//verify that uart is ready for transmission
	while((USARTx->ISR & USART_ISR_TEACK) == 0);
	
	//verify that uart is ready for reception 
	while((USARTx->ISR & USART_ISR_REACK) == 0);
}




void USART_Read (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes) {
	
	uint32_t i;
	
	for(i = 0; i < nBytes; i++) {
			while (!(USARTx->ISR & USART_ISR_RXNE));
		buffer[i] = USARTx->RDR;
	}
}




void USART_Write (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes) {
	
	uint32_t i;
	
	for(i = 0; i < nBytes; i++) {
			while (!(USARTx->ISR & USART_ISR_TXE));
			USARTx->TDR = buffer[i] & 0xFF;
	}
	
	//wait until TC bit is set
	while (!(USARTx->ISR & USART_ISR_TC));
	
	//writing 1 to the TCCF bit in ICR clears the TC bit in ISR
	USARTx->ICR |= USART_ICR_TCCF;
	
}