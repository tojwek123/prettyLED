/*
 * retainConfig.c
 *
 * Created: 13.02.2018 19:44:56
 *  Author: Tojwek
 */ 

#include "../config.h"

#include "../../retainMemory/retainMemory.h"

Config_t DefaultConfig = {
	.colors = { {0, 0, 0}, {0, 0, 0} }
};

void config_init()
{
	bool formatted = retainMemory_init(CONFIG_RETAIN_MEMORY_START_ADDRESS,
									   CONFIG_RETAIN_MEMORY_SIZE,
									   CONFIG_RETAIN_MEMORY_REDUNDANCY_LEVEL,
									   false);

	if (formatted)
	{
		config_save(&DefaultConfig);
	}
}

void config_read(Config_t const * config)
{
	retainMemory_blockingRead((void*)config, sizeof(Config_t));
}

void config_save(const Config_t const * config)
{
	retainMemory_blockingWrite((void*)config, sizeof(Config_t));
}