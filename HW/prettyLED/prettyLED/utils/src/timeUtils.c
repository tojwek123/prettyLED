/*
 * timeUtils.c
 *
 * Created: 13.01.2018 15:32:04
 *  Author: Tojwek
 */ 

#include "../timeUtils.h"

/* Private imports */
#include <avr/interrupt.h>
#include <util/atomic.h>

/* Private variables */
static uint32_t Tick;
static uint32_t SavedTick;

/* Interrupt handler */
ISR(TIMER0_COMPA_vect)
{
	++Tick;
}

void timeUtils_init()
{
	OCR0A = 250;
	TCCR0A |= (1 << WGM01);
	/* 64 psc */
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TIMSK0 |= (1 << OCIE0A);
}

uint32_t timeUtils_getTickMs()
{
	uint32_t ret;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ret = Tick;
	}
	return ret;
}

void timeUtils_tic()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SavedTick = Tick;
	}
}

uint32_t timeUtils_tocMs()
{	
	uint32_t ret;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		ret = Tick - SavedTick;
	}
	return ret;
}