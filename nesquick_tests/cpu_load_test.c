#include <stdio.h>
#include <assert.h>

#include "nes.h"
#include "cpu_test.h"

void test_cpu_load_instructions()
{
	test_cpu_lda_abs();
	test_cpu_lda_abs_x();
	test_cpu_lda_abs_y();
	test_cpu_lda_imm();
	test_cpu_lda_ind_x();
	test_cpu_lda_ind_y();
	test_cpu_lda_zpg();
	test_cpu_lda_zpg_x();

	test_cpu_ldx_abs();
	test_cpu_ldx_abs_y();
	test_cpu_ldx_imm();
	test_cpu_ldx_zpg();
	test_cpu_ldx_zpg_y();

	test_cpu_ldy_abs();
	test_cpu_ldy_abs_x();
	test_cpu_ldy_imm();
	test_cpu_ldy_zpg();
	test_cpu_ldy_zpg_x();
}

void test_cpu_lda_abs()
{
	printf("Testing LDA (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xAD; // 8000 LDA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag
	
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x80 - trigger negative flag

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_abs_x()
{
	printf("Testing LDA (absolute, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xBD; // 8000 LDA $8000, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66 - value to retrieve
	nes->bus->program_rom->data[0x0100] = 0x33; // 8100 0x33 - value to retrieve for new page
	
	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag
	
	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x01; // 8001 - test extra cycle for page boundary
	
	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0xFF; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->A == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	
	nesquick_nes_free(nes);
	
	printf("Instruction test passed.\n");
}

void test_cpu_lda_abs_y()
{
	printf("Testing LDA (absolute, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB9; // 8000 LDA $8000, Y
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66 - value to retrieve
	nes->bus->program_rom->data[0x0100] = 0x33; // 8100 0x33 - value to retrieve for new page

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x01; // 8001 - test extra cycle for page boundary

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0xFF; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->A == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_imm()
{
	printf("Testing LDA (immediate) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA9; // 8000 LDA #$33
	nes->bus->program_rom->data[0x0001] = 0x33; // 8001 

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->A == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x00;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->A == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x80;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->A == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_ind_x()
{
	printf("Testing LDA (indirect, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA1; // 8000 LDA ($01,X)
	nes->bus->program_rom->data[0x0001] = 0x01; // 8001

	nes->bus->program_rom->data[0x0899] = 0x55; // 8899 0x55

	nes->bus->system_ram->data[0x0010] = 0x99;
	nes->bus->system_ram->data[0x0011] = 0x88;
	
	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x0F;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(nes->cpu->A == 0x55);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->program_rom->data[0x0899] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x0F;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(nes->cpu->A == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->program_rom->data[0x0899] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x0F;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(nes->cpu->A == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_ind_y()
{
	printf("Testing LDA (indirect, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB1; // 8000 LDA ($01),Y
	nes->bus->program_rom->data[0x0001] = 0x01; // 8001

	nes->bus->program_rom->data[0x0899] = 0x55; // 8899 0x55
	nes->bus->program_rom->data[0x098F] = 0x66; // 898F 0x66 - for testing page boundary cycle

	nes->bus->system_ram->data[0x0001] = 0x90;
	nes->bus->system_ram->data[0x0002] = 0x88;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x09;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->A == 0x55);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->program_rom->data[0x0899] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x09;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->A == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->program_rom->data[0x0899] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x09;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->A == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0xFF;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(nes->cpu->A == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_zpg()
{
	printf("Testing LDA (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA5; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x01; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->A == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->A == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->A == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_lda_zpg_x()
{
	printf("Testing LDA (zero page, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB5; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x01;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0033] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0066] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x66;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->A == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldx_abs()
{
	printf("Testing LDX (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xAE; // 8000 LDA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x80 - trigger negative flag

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldx_abs_y()
{
	printf("Testing LDX (absolute, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xBE; // 8000 LDA $8000, Y
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66 - value to retrieve
	nes->bus->program_rom->data[0x0100] = 0x33; // 8100 0x33 - value to retrieve for new page

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x01; // 8001 - test extra cycle for page boundary

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0xFF; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->X == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldx_imm()
{
	printf("Testing LDX (immediate) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA2; // 8000 LDA #$33
	nes->bus->program_rom->data[0x0001] = 0x33; // 8001 

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->X == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x00;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->X == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x80;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->X == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldx_zpg()
{
	printf("Testing LDX (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA6; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x01; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->X == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->X == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->X == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldx_zpg_y()
{
	printf("Testing LDX (zero page, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB6; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x01;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0033] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0066] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x66;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->X == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldy_abs()
{
	printf("Testing LDY (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xAC; // 8000 LDA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x80 - trigger negative flag

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldy_abs_x()
{
	printf("Testing LDY (absolute, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xBC; // 8000 LDA $8000, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003
	nes->bus->program_rom->data[0x0003] = 0x66; // 8003 0x66 - value to retrieve
	nes->bus->program_rom->data[0x0100] = 0x33; // 8100 0x33 - value to retrieve for new page

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x66);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x00; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0003] = 0x80; // 8003 0x00 - trigger zero flag

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 3; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x01; // 8001 - test extra cycle for page boundary

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0xFF; // X offset
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(nes->cpu->Y == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldy_imm()
{
	printf("Testing LDY (immediate) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA0; // 8000 LDA #$33
	nes->bus->program_rom->data[0x0001] = 0x33; // 8001 

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->Y == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x00;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->Y == 0x00);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nes->bus->program_rom->data[0x0001] = 0x80;
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 2);
	assert(nes->cpu->Y == 0x80);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldy_zpg()
{
	printf("Testing LDY (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xA4; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x01; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->Y == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->Y == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0001] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(nes->cpu->Y == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_ldy_zpg_x()
{
	printf("Testing LDY (zero page, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB4; // 8000 LDA $01
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nes->bus->system_ram->data[0x0001] = 0x33;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x01;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x33);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0033] = 0x00;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x00);
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nes->bus->system_ram->data[0x0066] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x66;
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(nes->cpu->Y == 0x80);
	assert(nesquick_cpu_check_flag(nes->cpu, FLAG_NEGATIVE));
	assert(!nesquick_cpu_check_flag(nes->cpu, FLAG_ZERO));

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}