#include "cpu.h"

void nesquick_cpu_sta_abs(cpu_t *cpu)
{
	if(cpu == NULL)
	{
		return;
	}

	if(cpu->bus == NULL)
	{
		return;
	}
	
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->A);

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_sta_abs_x(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	cpu->address += cpu->X;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->A);

	cpu->PC += 3;
	cpu->cycle += 5;
}

void nesquick_cpu_sta_abs_y(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	cpu->address += cpu->Y;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->A);

	cpu->PC += 3;
	cpu->cycle += 5;
}

void nesquick_cpu_sta_ind_x(cpu_t *cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, offset);
	cpu->address |= nesquick_bus_read(cpu->bus, offset + 1) << 8;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->A);

	cpu->PC += 2;
	cpu->cycle += 6;
}

void nesquick_cpu_sta_ind_y(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, offset);
	cpu->address |= nesquick_bus_read(cpu->bus, offset + 1) << 8;
	cpu->address += cpu->Y;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->A);

	cpu->PC += 2;
	cpu->cycle += 6;
}

void nesquick_cpu_sta_zpg(cpu_t *cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}
	
	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	nesquick_bus_write(cpu->bus, offset, cpu->A);

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_sta_zpg_x(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	nesquick_bus_write(cpu->bus, offset, cpu->A);

	cpu->PC += 2;
	cpu->cycle += 4;
}

void nesquick_cpu_stx_abs(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->X);

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_stx_zpg(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	nesquick_bus_write(cpu->bus, offset, cpu->X);

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_stx_zpg_y(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->Y;
	nesquick_bus_write(cpu->bus, offset, cpu->X);

	cpu->PC += 2;
	cpu->cycle += 4;
}

void nesquick_cpu_sty_abs(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	nesquick_bus_write(cpu->bus, cpu->address, cpu->Y);

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_sty_zpg(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	nesquick_bus_write(cpu->bus, offset, cpu->Y);

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_sty_zpg_x(cpu_t* cpu)
{
	if (cpu == NULL)
	{
		return;
	}

	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	nesquick_bus_write(cpu->bus, offset, cpu->Y);

	cpu->PC += 2;
	cpu->cycle += 4;
}