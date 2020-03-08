#include <assert.h>
#include <stdio.h>
#include "nes.h"
#include "cpu_test.h"

void test_cpu_branch_instructions()
{
	test_cpu_bcc();
	test_cpu_bcs();
	test_cpu_beq();
	test_cpu_bmi();
	test_cpu_bne();
	test_cpu_bpl();
	test_cpu_bvc();
	test_cpu_bvs();
}

void test_cpu_bcc()
{
	printf("Testing BCC instruction...\n");
	
	nes_t *nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x90; // cycle 1 - 8000 
	nes->bus->program_rom->data[0x0001] = 0x40; // cycle 2 - 8001 BCC +0x40
												// cycle 3 - 8002 + 40 = 8042
												// 
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);
	
	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_bcs()
{
	printf("Testing BCS instruction...\n");
	
	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xB0;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	
	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);
	
	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);
	
	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_CARRY);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);
	
	printf("Instruction test passed.\n");
}

void test_cpu_beq()
{
	printf("Testing BEQ instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xF0;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_bmi()
{
	printf("Testing BMI instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x30;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_bne()
{
	printf("Testing BNE instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0xD0;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_ZERO);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_bpl()
{
	printf("Testing BPL instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x10;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_NEGATIVE);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_bvc()
{
	printf("Testing BVC instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x50;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}


void test_cpu_bvs()
{
	printf("Testing BVS instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x70;
	nes->bus->program_rom->data[0x0001] = 0x40;

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_clear_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x8040);
	assert(nes->cpu->cycle == 3);

	nes->bus->program_rom->data[0x0001] = 0xFF; // switch to -1 to test page boundaries

	nesquick_cpu_reset(nes->cpu);
	nesquick_cpu_set_flag(nes->cpu, FLAG_OVERFLOW);
	nesquick_cpu_execute_instruction(nes->cpu);

	assert(nes->cpu->PC == 0x7FFF);
	assert(nes->cpu->cycle == 4);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}