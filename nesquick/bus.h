#pragma once
#include "memory.h"

struct bus
{
	memory_t *system_ram;
	memory_t *program_rom;
	memory_t *program_ram;
	memory_t *character_rom;
};

typedef struct bus bus_t;

bus_t *nesquick_bus_init();
void nesquick_bus_free(bus_t *bus);
uint8_t nesquick_bus_read(bus_t *bus, uint16_t address);
void nesquick_bus_write(bus_t* bus, uint16_t address, uint8_t value);