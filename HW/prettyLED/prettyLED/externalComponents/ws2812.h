/*
 * ws2812.h
 *
 * Created: 11.01.2018 18:57:15
 *  Author: Tojwek
 */ 


#ifndef WS2812_H_
#define WS2812_H_

#include <avr/io.h>

#define WS2812_PORT		PORTD
#define WS2812_PIN		PIND
#define WS2812_DDR		DDRD
#define WS2812_PIN_NO	2

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ColorRGB_t;

void ws2812_init();
void ws2812_writeColorsRGB(const ColorRGB_t * const colors, const uint32_t len);
void ws2812_writeSequence(const uint8_t * const bytes, const uint32_t len);

#endif /* WS2812_H_ */