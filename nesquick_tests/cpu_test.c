#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "nes.h"
#include "cpu_test.h"
#include <stdio.h>


void test_cpu_init()
{
	
	
	cpu_t *cpu = nesquick_cpu_init();

	assert(cpu->A == 0 &&
		cpu->X == 0 &&
		cpu->Y == 0 &&
		cpu->P == 0 &&
		cpu->PC == 0 &&
		cpu->SP == 0);

	nesquick_cpu_free(cpu);
}

void test_cpu_restart()
{
	printf("Testing CPU restart...\n");
	
	nes_t *nes = nesquick_nes_init();
	assert(nes != NULL);
	assert(nes->cpu != NULL);
	assert(nes->bus != NULL);
	assert(nes->bus->system_ram != NULL);
	assert(nes->bus->program_rom != NULL);
	
	nes->bus->program_rom->data[0x7FFC] = 0x00; // reset vector low byte
	nes->bus->program_rom->data[0x7FFD] = 0x80; // reset vector high byte
	
	nesquick_cpu_reset(nes->cpu);
	assert(nes->cpu->PC == 0x8000);
	nesquick_nes_free(nes);

	printf("Test passed.\n");
}