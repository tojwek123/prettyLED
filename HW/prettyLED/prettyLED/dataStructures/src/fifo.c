/*
 * fifo.c
 *
 * Created: 12.01.2018 20:44:01
 *  Author: Tojwek
 */ 

 #include "../fifo.h"

 void fifo_init(Fifo_t * const f, const void * const mem, const uint32_t size)
 {
	f->mem = (uint8_t*)mem;
	f->size = size;
	f->bottom = 0;
	f->top = 0;
	f->bytesInUse = 0;
 }

 void fifo_push(Fifo_t * const f, const void * const element, const uint32_t size)
 {
	if (fifo_isFull(f))
	{
		f->bottom = (f->bottom + 1) % f->size;
	}
	
	for (uint32_t i = 0; i < size; ++i)
	{
		f->mem[f->top] = ((uint8_t*)element)[i];
		f->top = (f->top + 1) % f->size;

		if (fifo_isFull(f))
		{
			f->bottom = (f->bottom + 1) % f->size;
		}
		else
		{
			f->bytesInUse++;
		}
	}
 }

 uint32_t fifo_pop(Fifo_t * const f, void *element, const uint32_t size)
 {
	uint32_t bytesRead = 0;

	for (uint32_t i = 0; i < size; ++i)
	{
		if (fifo_isEmpty(f))
		{
			break;
		}
		else
		{
			bytesRead++;
		}

		((uint8_t*)element)[i] = f->mem[f->bottom];
		f->bottom = (f->bottom + 1) % f->size;
		f->bytesInUse--;
	}

	return bytesRead;
 }

 void fifo_flush(Fifo_t * const f)
 {
	f->bottom = 0;
	f->top = 0;
	f->bytesInUse = 0;
 }

 bool fifo_isEmpty(const Fifo_t * const f)
 {
	return 0 == f->bytesInUse;
 }

 bool fifo_isFull(const Fifo_t * const f)
 {
	return f->size == f->bytesInUse;
 }

 uint32_t fifo_availableBytes(const Fifo_t * const f)
 {
	return f->bytesInUse;
 }