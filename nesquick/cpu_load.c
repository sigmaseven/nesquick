#include "cpu.h"
#include "bus.h"

void nesquick_cpu_lda_abs(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	cpu->A = nesquick_bus_read(cpu->bus, cpu->address);

	if(cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if(nesquick_cpu_check_bit(cpu->A,0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}
	
	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_lda_abs_x(cpu_t *cpu)
{
	if(cpu->bus == NULL)
	{
		return;
	}
	
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	const uint8_t page = cpu->address >> 8;
	cpu->address += cpu->X;
	cpu->A = nesquick_bus_read(cpu->bus, cpu->address);

	if(cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if(nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	if(cpu->address >> 8 != page)
	{
		cpu->cycle++;
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_lda_abs_y(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	const uint8_t page = cpu->address >> 8;
	cpu->address += cpu->Y;
	cpu->A = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	if(cpu->address >> 8 != page)
	{
		cpu->cycle++;
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_lda_imm(cpu_t *cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
	cpu->A = nesquick_bus_read(cpu->bus, cpu->PC + 1);

	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}
	
	cpu->PC += 2;
	cpu->cycle += 2;
}

void nesquick_cpu_lda_ind_x(cpu_t *cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, offset);
	cpu->address |= nesquick_bus_read(cpu->bus, offset + 1) << 8;
	cpu->A = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}
	
	cpu->PC += 2;
	cpu->cycle += 6;
}

void nesquick_cpu_lda_ind_y(cpu_t *cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
    const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, offset);
	cpu->address |= nesquick_bus_read(cpu->bus, offset + 1) << 8;
	const uint8_t page = cpu->address >> 8;
	cpu->address += cpu->Y;
	cpu->A += nesquick_bus_read(cpu->bus, cpu->address);
	
	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}
	
	if(cpu->address >> 8 != page)
	{
		cpu->cycle++;
	}
	
	cpu->PC += 2;
	cpu->cycle += 5;
}

void nesquick_cpu_lda_zpg(cpu_t *cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->A = nesquick_bus_read(cpu->bus, offset);

	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_lda_zpg_x(cpu_t *cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}
	
	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	cpu->A = nesquick_bus_read(cpu->bus, offset);

	if (cpu->A == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->A, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}
	
	cpu->PC += 2;
	cpu->cycle += 4;
}

void nesquick_cpu_ldx_abs(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	cpu->X = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->X == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->X, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_ldx_abs_y(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	const uint8_t page = cpu->address >> 8;
	cpu->address += cpu->Y;
	cpu->X = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->X == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->X, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	if(cpu->address >> 8 != page)
	{
		cpu->cycle++;
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_ldx_imm(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->X = nesquick_bus_read(cpu->bus, cpu->PC + 1);

	if (cpu->X == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->X, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 2;
}

void nesquick_cpu_ldx_zpg(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->X = nesquick_bus_read(cpu->bus, offset);

	if (cpu->X == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->X, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_ldx_zpg_y(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->Y;
	cpu->X = nesquick_bus_read(cpu->bus, offset);

	if (cpu->X == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->X, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 4;
}

void nesquick_cpu_ldy_abs(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	cpu->Y = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->Y == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->Y, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_ldy_abs_x(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->address = 0x0000;
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->address |= nesquick_bus_read(cpu->bus, cpu->PC + 2) << 8;
	const uint8_t page = cpu->address >> 8;
	cpu->address += cpu->X;
	cpu->Y = nesquick_bus_read(cpu->bus, cpu->address);

	if (cpu->Y == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->Y, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	if (cpu->address >> 8 != page)
	{
		cpu->cycle++;
	}

	cpu->PC += 3;
	cpu->cycle += 4;
}

void nesquick_cpu_ldy_imm(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	cpu->Y = nesquick_bus_read(cpu->bus, cpu->PC + 1);

	if (cpu->Y == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->Y, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 2;
}

void nesquick_cpu_ldy_zpg(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	const uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	cpu->Y = nesquick_bus_read(cpu->bus, offset);

	if (cpu->Y == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->Y, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 3;
}

void nesquick_cpu_ldy_zpg_x(cpu_t* cpu)
{
	if (cpu->bus == NULL)
	{
		return;
	}

	uint8_t offset = nesquick_bus_read(cpu->bus, cpu->PC + 1);
	offset += cpu->X;
	cpu->Y = nesquick_bus_read(cpu->bus, offset);

	if (cpu->Y == 0)
	{
		nesquick_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_ZERO);
	}

	if (nesquick_cpu_check_bit(cpu->Y, 0x80))
	{
		nesquick_cpu_set_flag(cpu, FLAG_NEGATIVE);
	}
	else
	{
		nesquick_cpu_clear_flag(cpu, FLAG_NEGATIVE);
	}

	cpu->PC += 2;
	cpu->cycle += 4;
}

