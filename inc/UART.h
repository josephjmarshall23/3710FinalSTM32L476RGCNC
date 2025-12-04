#pragma once
#include "stm32l476xx.h"
#include <stdint.h>

void USART_Init (USART_TypeDef * USARTx);
void USART_Read (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);
void USART_Write (USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);
void uart_enable();
void uart_disable();
int uart_available();
char uart_read();
uint8_t uart_write(char *msg, uint32_t len);
