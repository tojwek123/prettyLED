/*
 * retainMemory.c
 *
 * Created: 09.02.2018 20:36:55
 *  Author: Tojwek
 */ 

#include "../retainMemory.h"
#include "../../internalComponents/eeprom.h"

//#define DEBUG_LOG

#ifdef DEBUG_LOG
	#include "../../internalComponents/uart.h"
	#include <stdio.h>
#endif

typedef struct  
{
	uint16_t startAddress;
	uint16_t size;
	uint16_t redundancyLevel;
} RetainMemory_Header_t;

typedef struct 
{
	uint32_t freshness;
	uint16_t address;
} RetainMemory_Block_t;

static RetainMemory_Header_t Header;
static RetainMemory_Block_t FreshestBlock;
static uint16_t FreshestBlockAddress;

static inline uint16_t getBlocksFirstAddress(const RetainMemory_Header_t * const pHeader)
{
	return pHeader->startAddress + sizeof(RetainMemory_Header_t);
}

static inline uint16_t getBlocksLastAddress(const RetainMemory_Header_t * const pHeader)
{
	return getBlocksFirstAddress(pHeader) + (pHeader->redundancyLevel - 1) * sizeof(RetainMemory_Block_t);
}

static inline uint16_t getDataMemoryFirstAddress(const RetainMemory_Header_t * const pHeader)
{
	return getBlocksLastAddress(pHeader) + sizeof(RetainMemory_Block_t);
}

static inline uint16_t getDataMemoryLastAddress(const RetainMemory_Header_t * const pHeader)
{
	return pHeader->startAddress + pHeader->size - 1;
}

bool retainMemory_init(const uint16_t startAddress, const uint16_t size, const uint16_t redundancyLevel, const bool forceFormat)
{
	bool formatted = false;

	/* Read header */
	RetainMemory_Header_t headerInMemory;
	eeprom_blockingReadMultipleBytes(startAddress, (uint8_t*)(&headerInMemory), sizeof(headerInMemory));

	Header.startAddress = startAddress;
	Header.size = size;
	Header.redundancyLevel = redundancyLevel;

	FreshestBlockAddress = getBlocksFirstAddress(&Header);
	FreshestBlock.address = getDataMemoryFirstAddress(&Header);
	FreshestBlock.freshness = 0;

	/* Check if requested retain memory conf. is the same */
	if (!forceFormat &&
		headerInMemory.startAddress == startAddress &&
		headerInMemory.size == size &&
		headerInMemory.redundancyLevel == redundancyLevel)
	{
		/* Requested retain memory conf. is the same */

#ifdef DEBUG_LOG
			uart_blockingWriteStr("\r\nRetain memory conf. is the same\r\n");
#endif

		/* Find the freshest block */
		uint16_t address = startAddress + sizeof(Header);
		RetainMemory_Block_t block;

		for (uint16_t i = 0; i < redundancyLevel; ++i)
		{
			eeprom_blockingReadMultipleBytes(address, (uint8_t*)(&block), sizeof(block));
			
			if (block.freshness > FreshestBlock.freshness)
			{
				FreshestBlock = block;
			}

			address += sizeof(RetainMemory_Block_t);
		}
	}
	else
	{
		/* Requested retain memory conf. is not the same - write new header */
#ifdef DEBUG_LOG
		uart_blockingWriteStr("\r\nFormatting memory\r\n");
#endif

		eeprom_blockingWriteMultipleBytes(startAddress, (uint8_t*)(&Header), sizeof(Header));

		/* Set rest of the memory to zero */
		for (uint16_t address = getBlocksFirstAddress(&Header); address < startAddress + size; ++address)
		{
			eeprom_blockingWriteByte(address, 0U);
		}

		formatted = true;
	}

#ifdef DEBUG_LOG
	char buffer[60];
	sprintf(buffer, "Header.startAddress = %d\r\n", Header.startAddress);
	uart_blockingWriteStr(buffer);
	sprintf(buffer, "Header.size = %d\r\n", Header.size);
	uart_blockingWriteStr(buffer);
	sprintf(buffer, "Header.redundancyLevel = %d\r\n", Header.redundancyLevel);
	uart_blockingWriteStr(buffer);
	sprintf(buffer, "FreshestBlock.address = %d\r\n", FreshestBlock.address);
	uart_blockingWriteStr(buffer);
	sprintf(buffer, "FreshestBlock.freshness = %d\r\n", FreshestBlock.freshness);
	uart_blockingWriteStr(buffer);
#endif

	return formatted;
}

void retainMemory_blockingWrite(const void * const data, const uint16_t size)
{
	/* Get address of the next redundancy block */
	FreshestBlockAddress = FreshestBlockAddress + sizeof(FreshestBlock);
	
	if (FreshestBlockAddress > getBlocksLastAddress(&Header))
	{
		FreshestBlockAddress = getBlocksFirstAddress(&Header);
	}

	/* Update redundancy block */
	++FreshestBlock.freshness;
	++FreshestBlock.address;

	if (FreshestBlock.address > getDataMemoryLastAddress(&Header))
	{
		FreshestBlock.address = getDataMemoryFirstAddress(&Header);
	}

	/* Write redundancy block */
	eeprom_blockingWriteMultipleBytes(FreshestBlockAddress, (uint8_t*)(&FreshestBlock), sizeof(FreshestBlock));

	/* Write memory */	
	uint16_t address = FreshestBlock.address;
	for (uint16_t i = 0; i < size; ++i)
	{
		eeprom_blockingWriteByte(address, *((uint8_t*)data + i));

		++address;

		if (address > getDataMemoryLastAddress(&Header))
		{
			address = getDataMemoryFirstAddress(&Header);
		}
	}
}

void retainMemory_blockingRead(void * const data, const uint16_t size)
{	
	uint16_t address = FreshestBlock.address;

	for (uint16_t i = 0; i < size; ++i)
	{
		*((uint8_t*)data + i) = eeprom_blockingReadByte(address);

		++address;

		if (address > getDataMemoryLastAddress(&Header))
		{
			address = getDataMemoryFirstAddress(&Header);
		}
	}
}