#include "cpu.h"

void nesquick_cpu_clc(cpu_t *cpu)
{
	nesquick_cpu_clear_flag(cpu, FLAG_CARRY);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_cld(cpu_t *cpu)
{
	nesquick_cpu_clear_flag(cpu, FLAG_DECIMAL);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_cli(cpu_t *cpu)
{
	nesquick_cpu_clear_flag(cpu, FLAG_INTERRUPT);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_clv(cpu_t *cpu)
{
	nesquick_cpu_clear_flag(cpu, FLAG_OVERFLOW);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_sec(cpu_t *cpu)
{
	nesquick_cpu_set_flag(cpu, FLAG_CARRY);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_sed(cpu_t *cpu)
{
	nesquick_cpu_set_flag(cpu, FLAG_CARRY);
	cpu->PC++;
	cpu->cycle += 2;
}

void nesquick_cpu_sei(cpu_t* cpu)
{
	nesquick_cpu_set_flag(cpu, FLAG_INTERRUPT);
	cpu->PC++;
	cpu->cycle += 2;
}