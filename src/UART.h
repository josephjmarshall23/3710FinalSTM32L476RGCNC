#pragma once
#include "stm32l476xx.h"

void USART_Init (USART_TypeDef * USARTx);
void USART_Read (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);
void USART_Write (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);