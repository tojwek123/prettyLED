/*
 * uart.h
 *
 * Created: 11.01.2018 20:27:41
 *  Author: Tojwek
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

void uart_init();
bool uart_canReadLine();
uint32_t uart_availableBytes();
void uart_readLine(char * const line, const uint32_t maxSize);
void uart_flush();
void uart_blockingWriteChar(const char ch);
void uart_blockingWriteStr(const char * const str);
void uart_blockingWriteStrN(const char * const str, const uint32_t len);


#endif /* UART_H_ */