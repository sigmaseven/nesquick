#include "nes.h"
#include <string.h>
#include <stdlib.h>

nes_t *nesquick_nes_init()
{
	memory_t *prg_rom = nesquick_memory_init(MEMORY_TYPE_ROM, 0x8000);
	memset(prg_rom->data, 0, 0x8000);
	
	bus_t* bus = nesquick_bus_init();
	bus->program_rom = prg_rom;
	
	cpu_t* cpu = nesquick_cpu_init();
	cpu->bus = bus;

	nes_t* nes = (nes_t*)malloc(sizeof(nes_t));
	nes->cpu = cpu;
	nes->bus = bus;
	
	return nes;
}

void nesquick_nes_free(nes_t* nes)
{
	nesquick_bus_free(nes->bus);
	nesquick_cpu_free(nes->cpu);
}
