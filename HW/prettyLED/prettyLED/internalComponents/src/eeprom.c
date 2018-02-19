/*
 * eeprom.c
 *
 * Created: 09.02.2018 20:00:27
 *  Author: Tojwek
 */ 

#include "../eeprom.h"
#include <avr/io.h>
#include <util/atomic.h>

void eeprom_blockingWriteByte(const uint16_t address, const uint8_t byte)
{
	/* Wait for previous write to complete */
	while (EECR & (1 << EEPE)) {}

	/* Set address to write */
	EEAR = address;

	/* Set data to write */
	EEDR = byte;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		/* Master write enable */
		EECR |= (1 << EEMPE);

		/* Start writing */
		EECR |= (1 << EEPE);
	}
}

void eeprom_blockingWriteMultipleBytes(const uint16_t address, const uint8_t * const bytes, const uint16_t len)
{
	for (uint16_t i = 0; i < len; ++i)
	{
		eeprom_blockingWriteByte(address + i, bytes[i]);
	}
}

uint8_t eeprom_blockingReadByte(const uint16_t address)
{
	/* Wait for previous write to complete */
	while (EECR & (1 << EEPE)) {}

	/* Set address to read */
	EEAR = address;

	/* Read data - will be available immediately */
	EECR |= (1 << EERE);

	return EEDR;
}

void eeprom_blockingReadMultipleBytes(const uint16_t address, uint8_t * const bytes, const uint16_t len)
{
	for (uint16_t i = 0; i < len; ++i)
	{
		bytes[i] = eeprom_blockingReadByte(address + i);
	}
}