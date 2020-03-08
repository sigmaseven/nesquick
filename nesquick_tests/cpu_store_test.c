#include <stdio.h>
#include <assert.h>

#include "nes.h"
#include "cpu_test.h"

void test_cpu_store_instructions()
{
	test_cpu_sta_abs();
	test_cpu_sta_abs_x();
	test_cpu_sta_abs_y();
	test_cpu_sta_ind_x();
	test_cpu_sta_ind_y();
	test_cpu_sta_zpg();
	test_cpu_sta_zpg_x();

	test_cpu_stx_abs();
	test_cpu_stx_zpg();
	test_cpu_stx_zpg_y();

	test_cpu_sty_abs();
	test_cpu_sty_zpg();
	test_cpu_sty_zpg_x();
}

void test_cpu_sta_abs()
{
	printf("Testing STA (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x8D; // 8000 STA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8003);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);
	
	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_abs_x()
{
	printf("Testing STA (absolute, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x9D; // 8000 STA $8000, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8000

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nes->cpu->X = 0x99;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8099);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_abs_y()
{
	printf("Testing STA (absolute, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x99; // 8000 STA $8000, Y
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8000

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nes->cpu->Y = 0x99;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8099);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 5);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_ind_x()
{
	printf("Testing STA (indirect, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x81; // 8000 STA ($00, X)
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001
	
	nes->bus->system_ram->data[0x66] = 0x99;
	nes->bus->system_ram->data[0x67] = 0x80;
	
	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nes->cpu->X = 0x66;
	nesquick_cpu_execute_instruction(nes->cpu);
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8099);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_ind_y()
{
	printf("Testing STA (indirect, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x91; // 8000 STA ($00), y
	nes->bus->program_rom->data[0x0001] = 0x66; // 8001

	nes->bus->system_ram->data[0x66] = 0x00;
	nes->bus->system_ram->data[0x67] = 0x80;

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nes->cpu->Y = 0x99;
	nesquick_cpu_execute_instruction(nes->cpu);
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8099);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 6);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_zpg()
{
	printf("Testing STA (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x85; // 8000 STA $66
	nes->bus->program_rom->data[0x0001] = 0x66; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->A = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sta_zpg_x()
{
	printf("Testing STA (zero page, x-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x95; // 8000 STA $00, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x66;
	nes->cpu->A = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);
	
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_stx_abs()
{
	printf("Testing STX (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x8E; // 8000 STA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8003);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_stx_zpg()
{
	printf("Testing STX (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x86; // 8000 STA $66
	nes->bus->program_rom->data[0x0001] = 0x66; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_stx_zpg_y()
{
	printf("Testing STX (zero page, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x96; // 8000 STA $00, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x66;
	nes->cpu->X = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sty_abs()
{
	printf("Testing STY (absolute) instruction...\n");

	nes_t* nes = nesquick_nes_init();
	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x8C; // 8000 STA $8003
	nes->bus->program_rom->data[0x0001] = 0x03; // 8001 
	nes->bus->program_rom->data[0x0002] = 0x80; // 8002 0x8003

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x8003);

	assert(nes->cpu->PC == 0x8003);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sty_zpg()
{
	printf("Testing STY (zero page) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x84; // 8000 STA $66
	nes->bus->program_rom->data[0x0001] = 0x66; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->Y = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);
	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 3);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}

void test_cpu_sty_zpg_x()
{
	printf("Testing STY (zero page, y-indexed) instruction...\n");

	nes_t* nes = nesquick_nes_init();

	nes->bus->program_rom->data[0x7FFC] = 0x00;
	nes->bus->program_rom->data[0x7FFD] = 0x80;

	nes->bus->program_rom->data[0x0000] = 0x94; // 8000 STA $00, X
	nes->bus->program_rom->data[0x0001] = 0x00; // 8001

	nesquick_cpu_reset(nes->cpu);
	nes->cpu->X = 0x66;
	nes->cpu->Y = 0x33;
	nesquick_cpu_execute_instruction(nes->cpu);

	const uint8_t byte = nesquick_bus_read(nes->cpu->bus, 0x0066);

	assert(nes->cpu->PC == 0x8002);
	assert(nes->cpu->cycle == 4);
	assert(byte == 0x33);

	nesquick_nes_free(nes);

	printf("Instruction test passed.\n");
}