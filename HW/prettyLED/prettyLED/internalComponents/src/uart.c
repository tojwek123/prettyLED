/*
 * uart.c
 *
 * Created: 11.01.2018 20:27:30
 *  Author: Tojwek
 */ 

 #include "../uart.h"

 /* Private imports */
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/atomic.h>
 #include "../../dataStructures/fifo.h"

 /* Private constants */
 #define NULL 0
 #define RX_BUFFER_SIZE 128

 /* Private variables */
 static char RxBufferMem[RX_BUFFER_SIZE];
 static Fifo_t RxBuffer;
 static uint32_t LinesRead;

 /* Interrupt handler */
 ISR(USART_RX_vect)
 {
	char ch = UDR0;
	fifo_push(&RxBuffer, &ch, sizeof(ch));

	if ('\n' == ch)
	{
		++LinesRead;
	}
 }


 /* Public functions implementation */
 void uart_init()
 {
	/* Uart runs with 9600 baud rate - values for UBRR are taken from datasheet table */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = 103;

	fifo_init(&RxBuffer, RxBufferMem, RX_BUFFER_SIZE);
 }

 bool uart_canReadLine()
 {
	bool ret;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ret = LinesRead > 0;
	}
	return ret;
 }

 uint32_t uart_availableBytes()
 {
	uint32_t ret;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ret = fifo_availableBytes(&RxBuffer);
	}

	return ret;
 }

 uint8_t uart_readByte()
 {
	uint8_t ret;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		fifo_pop(&RxBuffer, &ret, sizeof(ret));
	}

	return ret;
 }

 void uart_readLine(char * const line, const uint32_t maxSize)
 {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint32_t i;
		for (i = 0; i < maxSize - 1; ++i)
		{
			char ch;

			if (0 != fifo_pop(&RxBuffer, &ch, sizeof(ch)))
			{
				if ('\n' != ch)
				{
					line[i] = ch;
				}
				else
				{
					--LinesRead;
					break;
				}
			}
			else
			{
				break;
			}
		}
		line[i] = '\0';
	}
 }

 void uart_flush()
 {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		fifo_flush(&RxBuffer);
		LinesRead = 0;
	}
 }

 void uart_blockingWriteChar(const char ch)
 {
	while (!(UCSR0A & (1 << UDRE0))) {}
	UDR0 = ch;
 }

 void uart_blockingWriteStr(const char * const str)
 {
	uint32_t i = 0;

	while ('\0' != str[i])
	{
		uart_blockingWriteChar(str[i++]);
	}
 }

 void uart_blockingWriteStrN(const char * const str, const uint32_t len)
 {
	for (uint32_t i = 0; i < len; ++i)
	{
		uart_blockingWriteChar(str[i]);
	}
 }