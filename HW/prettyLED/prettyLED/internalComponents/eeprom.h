/*
 * eeprom.h
 *
 * Created: 09.02.2018 20:00:16
 *  Author: Tojwek
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

#define EEPROM_SIZE 1024U

void eeprom_blockingWriteByte(const uint16_t address, const uint8_t byte);
void eeprom_blockingWriteMultipleBytes(const uint16_t address, const uint8_t * const bytes, const uint16_t len);

uint8_t eeprom_blockingReadByte(const uint16_t address);
void eeprom_blockingReadMultipleBytes(const uint16_t address, uint8_t * const bytes, const uint16_t len);

#endif /* EEPROM_H_ */