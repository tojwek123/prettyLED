/*
 * retainMemory.h
 *
 * Created: 09.02.2018 20:36:45
 *  Author: Tojwek
 */ 


#ifndef RETAINMEMORY_H_
#define RETAINMEMORY_H_

#define RETAINMEMORY_SIZE EEPROM_SIZE

#include <stdint.h>
#include <stdbool.h>

bool retainMemory_init(const uint16_t startAddress, const uint16_t size, const uint16_t redundancyLevel, const bool forceFormat);
void retainMemory_blockingWrite(const void * const data, const uint16_t size);
void retainMemory_blockingRead(void * const data, const uint16_t size);


#endif /* RETAINMEMORY_H_ */