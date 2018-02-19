/*
 * retainConfig.h
 *
 * Created: 13.02.2018 19:44:46
 *  Author: Tojwek
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "../externalComponents/ws2812.h"

#define CONFIG_LEDS_CNT							2
#define CONFIG_RETAIN_MEMORY_START_ADDRESS		0
#define CONFIG_RETAIN_MEMORY_SIZE				1024
#define CONFIG_RETAIN_MEMORY_REDUNDANCY_LEVEL	3

typedef struct  
{
	ColorRGB_t colors[CONFIG_LEDS_CNT];
} Config_t;

extern Config_t DefaultConfig;

void config_init();
void config_read(Config_t const * config);
void config_save(const Config_t const * config);

#endif /* CONFIG_H_ */