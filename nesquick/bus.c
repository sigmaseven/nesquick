#include <stdlib.h>
#include "bus.h"
#include <string.h>

bus_t *nesquick_bus_init()
{
	bus_t* bus = (bus_t*)malloc(sizeof(bus_t));

	if (bus != NULL)
	{
		bus->system_ram = nesquick_memory_init(MEMORY_TYPE_RAM, 0x800);
		memset(bus->system_ram->data, 0, 0x800);
		bus->program_rom = NULL;
		bus->character_rom = NULL;
		bus->program_ram = NULL;
	}
	
	return bus;
}

void nesquick_bus_free(bus_t *bus)
{
	free(bus->system_ram);
	free(bus);
}

uint8_t nesquick_bus_read(bus_t *bus, uint16_t address)
{
	if(address < 0x8000)
	{
		address &= 0x07FF;
		if(address <= bus->system_ram->size)
		{
			return bus->system_ram->data[address];
		}
	}
	else if(address >= 0x8000)
	{
		if((address - 0x8000) <= bus->program_rom->size)
		{
			return bus->program_rom->data[address - 0x8000];
		}
	}
	
	return 0;
}

void nesquick_bus_write(bus_t *bus, uint16_t address, uint8_t value)
{
	if(address < 0x8000)
	{
		address &= 0x07FF;
		if(address <= bus->system_ram->size)
		{
			bus->system_ram->data[address] = value;
		}
	}
	else if (address >= 0x8000)
	{
		if ((address - 0x8000) <= bus->program_rom->size)
		{
			bus->program_rom->data[address - 0x8000] = value;
		}
	}
}