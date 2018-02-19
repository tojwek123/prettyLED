/*
 * prettyLED.c
 *
 * Created: 11.01.2018 18:51:17
 * Author : Tojwek
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "externalComponents/ws2812.h"
#include "internalComponents/uart.h"
#include "utils/timeUtils.h"
#include "config/config.h"

#define UART_LINE_MAX_SIZE		128
#define SAVE_COLOR_TIMEOUT_MS	3000

int main(void)
{
	uart_init();   
    ws2812_init();
	timeUtils_init();
	config_init();

	sei();
	
	Config_t config;

	config_read(&config);
	ws2812_writeColorsRGB(config.colors, CONFIG_LEDS_CNT);

	char uartLine[UART_LINE_MAX_SIZE];
	bool saveColor = false;

    while (1) 
    {
		if (uart_canReadLine())
		{
			ColorRGB_t readColor_1 = {};
			ColorRGB_t readColor_2 = {};

			uart_readLine(uartLine, UART_LINE_MAX_SIZE);
			uart_blockingWriteStr(uartLine);
			uart_blockingWriteStr("\r\n");

			if (6 == sscanf(uartLine, "%hhu,%hhu,%hhu %hhu,%hhu,%hhu", 
							&readColor_1.r, &readColor_1.g, &readColor_1.b,
							&readColor_2.r, &readColor_2.g, &readColor_2.b))
			{
				config.colors[0] = readColor_1;
				config.colors[1] = readColor_2;
				ws2812_writeColorsRGB(config.colors, CONFIG_LEDS_CNT);
				saveColor = true;
				timeUtils_tic();
			}
		}

		if (saveColor)
		{
			if (timeUtils_tocMs() > SAVE_COLOR_TIMEOUT_MS)
			{
				saveColor = false;
				config_save(&config);
				uart_blockingWriteStr("Config saved!\r\n");
			}
		}
    }
}

