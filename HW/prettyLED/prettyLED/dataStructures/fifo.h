/*
 * fifo.h
 *
 * Created: 12.01.2018 20:43:51
 *  Author: Tojwek
 */ 


#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t *mem;
	uint32_t size;
	uint32_t top;
	uint32_t bottom;
	uint32_t bytesInUse;
} Fifo_t;

void fifo_init(Fifo_t * const f,const void * const mem, const uint32_t size);
void fifo_push(Fifo_t * const f, const void * const element, const uint32_t size);
uint32_t fifo_pop(Fifo_t * const f, void *element, const uint32_t size);
void fifo_flush(Fifo_t * const f);
bool fifo_isEmpty(const Fifo_t * const f);
bool fifo_isFull(const Fifo_t * const f);
uint32_t fifo_availableBytes(const Fifo_t * const f);

#endif /* FIFO_H_ */