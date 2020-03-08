#pragma once
#include "bus.h"
#include "cpu.h"

struct nes
{
	cpu_t *cpu;
	bus_t *bus;
};

typedef struct nes nes_t;

nes_t *nesquick_nes_init();
void nesquick_nes_free(nes_t *nes);