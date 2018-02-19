/*
 * ws2812.c
 *
 * Created: 11.01.2018 18:57:40
 *  Author: Tojwek
 */ 

#include "../ws2812.h"

/* Private imports */
#include <util/delay.h>
#include <util/atomic.h>

/* Private function prototypes */
static void impl_ws2812_reset();
static void impl_ws2812_writeBit(const uint8_t bit);
static void impl_ws2812_writeByte(const uint8_t byte);

/* Public functions implementation */
void ws2812_init()
{
	WS2812_DDR |= (1 << WS2812_PIN_NO);
}

void ws2812_writeColorsRGB(const ColorRGB_t * const colors, const uint32_t len)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint32_t i = 0; i < len; ++i)
		{
			impl_ws2812_writeByte(colors[i].g);
			impl_ws2812_writeByte(colors[i].r);
			impl_ws2812_writeByte(colors[i].b);
		}
		impl_ws2812_reset();
	}
}

void ws2812_writeSequence(const uint8_t * const bytes, const uint32_t len)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		for (uint32_t i = 0; i < len; ++i)
		{
			impl_ws2812_writeByte(bytes[i]);
		}
		impl_ws2812_reset();
	}
}

/* Private functions implementation */
static void impl_ws2812_reset()
{
	WS2812_PORT &= ~(1 << WS2812_PIN_NO);
	_delay_us(50);
}

static void impl_ws2812_writeBit(const uint8_t bit)
{
	WS2812_PORT |= (1 << WS2812_PIN_NO);

	if (bit)
	{
		/* Delay 750 ns (spec. says approx. 700 ns) */
		asm volatile(
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
		);

		WS2812_PORT &= ~(1 << WS2812_PIN_NO);

		/* Delay 625 ns (spec. says 600 ns) */
		asm volatile(
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
		);
	}
	else
	{
		/* Delay 125 ns (spec. says 350 ns - IT DOES NOT WORK!) */
		asm volatile(
			"    nop"	"\n"
			"    nop"	"\n"
		);

		WS2812_PIN |= (1 << WS2812_PIN_NO);

		/* Delay 812,5 ns (spec. says 800 ns) */
		asm volatile(
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
			"    nop"	"\n"
		);
	}
}

static void impl_ws2812_writeByte(const uint8_t byte)
{
	uint8_t mask = 0b10000000;

	while (mask != 0)
	{
		impl_ws2812_writeBit(byte & mask);
		mask >>= 1;
	}
}