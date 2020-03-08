#include "cpu.h"

void nesquick_cpu_bcc(cpu_t* cpu)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
	if(!nesquick_cpu_check_flag(cpu, FLAG_CARRY))
	{	
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;
	
    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bcs(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);

	if(nesquick_cpu_check_flag(cpu, FLAG_CARRY))
	{	
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;
	
    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_beq(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
    // is zero flag set?
	if(nesquick_cpu_check_flag(cpu, FLAG_ZERO))
	{
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;
	
    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bne(cpu_t* cpu, uint8_t value) {
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
    // is zero flag clear?
	if(!nesquick_cpu_check_flag(cpu, FLAG_ZERO))
	{
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;
	
    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bmi(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
    // is negative flag set?
	if(nesquick_cpu_check_flag(cpu, FLAG_NEGATIVE))
	{
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;
	
    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bpl(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
    // is negative flag cleared?
	if(!nesquick_cpu_check_flag(cpu, FLAG_NEGATIVE))
	{
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;

    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bvc(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	
    // is overflow flag clear?
	if(!nesquick_cpu_check_flag(cpu, FLAG_OVERFLOW))
	{
        cpu->PC += offset;
	}
	else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;

    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}

void nesquick_cpu_bvs(cpu_t* cpu, uint8_t value)
{
    uint8_t page = cpu->PC >> 8;
    int8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);

    // is overflow flag set?
    if (nesquick_cpu_check_flag(cpu, FLAG_OVERFLOW))
    {
        cpu->PC += offset;
    }
    else
    {
        cpu->PC += 2;
    }

    cpu->cycle += 3;

    if ((cpu->PC >> 8) != page)
    {
        cpu->cycle++;
    }
}