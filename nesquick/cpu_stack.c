#include "cpu.h"
#include "bus.h"

void nesquick_cpu_pha(cpu_t *cpu)
{
	uint16_t address = cpu->SP | 0x0100;
	address &= 0x01FF;
	nesquick_bus_write(cpu->bus, address, cpu->A);
	cpu->SP--;
	cpu->PC += 1;
	cpu->cycle += 3;
}

void nesquick_cpu_php(cpu_t *cpu)
{
	uint16_t address = cpu->SP | 0x0100;
	address &= 0x01FF;
	nesquick_bus_write(cpu->bus, address, cpu->P);
	cpu->SP--;
	cpu->PC += 1;
	cpu->cycle += 3;
}

void nesquick_cpu_pla(cpu_t *cpu)
{
	uint16_t address = cpu->SP | 0x0100;
	address &= 0x01FF;
	cpu->A = nesquick_bus_read(cpu->bus, address);
	cpu->SP++;
	cpu->PC += 1;
	cpu->cycle += 4;
}

void nesquick_cpu_plp(cpu_t *cpu)
{
	uint16_t address = cpu->SP | 0x0100;
	address &= 0x01FF;
	cpu->P = nesquick_bus_read(cpu->bus, address);
	cpu->SP++;
	cpu->PC += 1;
	cpu->cycle += 4;
}

void nesquick_cpu_tax(cpu_t *cpu)
{
	cpu->X = cpu->A;
	cpu->PC += 1;
	cpu->cycle += 2;
}

void nesquick_cpu_tay(cpu_t* cpu)
{
	cpu->Y = cpu->A;
	cpu->PC += 1;
	cpu->cycle += 2;
}

void nesquick_cpu_tsx(cpu_t* cpu)
{
	cpu->X = cpu->SP;
	cpu->PC += 1;
	cpu->cycle += 2;
}

void nesquick_cpu_txa(cpu_t* cpu)
{
	cpu->A = cpu->X;
	cpu->PC += 1;
	cpu->cycle += 2;
}

void nesquick_cpu_txs(cpu_t* cpu)
{
	cpu->SP = cpu->X;
	cpu->PC += 1;
	cpu->cycle += 2;
}

void nesquick_cpu_tya(cpu_t* cpu)
{
	cpu->A = cpu->Y;
	cpu->PC += 1;
	cpu->cycle += 2;
}