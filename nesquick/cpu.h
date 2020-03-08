#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "bus.h"

#define VECTOR_RESET 0xFFFC;

enum address_mode
{
    MODE_ACCUMULATOR,
    MODE_ABSOLUTE,
    MODE_ABSOLUTE_X,
    MODE_ABSOLUTE_Y,
    MODE_IMMEDIATE,
    MODE_IMPLIED,
    MODE_INDIRECT,
    MODE_INDIRECT_X,
    MODE_INDIRECT_Y,
    MODE_RELATIVE,
    MODE_ZEROPAGE,
    MODE_ZEROPAGE_X,
    MODE_ZEROPAGE_Y,
};

enum flag
{
	FLAG_CARRY     = 0b00000001,
	FLAG_ZERO      = 0b00000010,
	FLAG_INTERRUPT = 0b00000100,
	FLAG_DECIMAL   = 0b00001000,
	FLAG_OVERFLOW  = 0b01000000,
	FLAG_NEGATIVE  = 0b10000000,
};

enum opcode
{
    OPCODE_ADC,
	OPCODE_AND,
	OPCODE_ASL,
	OPCODE_BCC,
	OPCODE_BCS,
	OPCODE_BEQ,
	OPCODE_BIT,
	OPCODE_BMI,
	OPCODE_BNE,
	OPCODE_BPL,
	OPCODE_BRK,
	OPCODE_BVC,
	OPCODE_BVS,
	OPCODE_CLC,
	OPCODE_CLD,
	OPCODE_CLI,
	OPCODE_CLV,
	OPCODE_CMP,
	OPCODE_CPX,
	OPCODE_CPY,
	OPCODE_DEC,
	OPCODE_DEX,
	OPCODE_DEY,
	OPCODE_EOR,
	OPCODE_INC,
	OPCODE_INX,
	OPCODE_INY,
	OPCODE_JMP,
	OPCODE_JSR,
	OPCODE_LDA,
	OPCODE_LDX,
	OPCODE_LDY,
	OPCODE_LSR,
	OPCODE_NOP,
	OPCODE_ORA,
	OPCODE_PHA,
	OPCODE_PHP,
	OPCODE_PLA,
	OPCODE_PLP,
	OPCODE_ROL,
	OPCODE_ROR,
	OPCODE_RTI,
	OPCODE_RTS,
	OPCODE_SBC,
	OPCODE_SEC,
	OPCODE_SED,
	OPCODE_SEI,
	OPCODE_STA,
	OPCODE_STX,
	OPCODE_STY,
	OPCODE_TAX,
	OPCODE_TAY,
	OPCODE_TSX,
	OPCODE_TXA,
	OPCODE_TXS,
	OPCODE_TYA,
	OPCODE_ILLEGAL,
	OPCODE_NONE,
};

struct instruction
{
	enum opcode operation;
	enum address_mode mode;
};

struct CPU
{
    // Registers
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t P;
	uint8_t data;
    uint16_t PC;
    uint16_t SP;
	
	// Internal State
	bool debug;
	enum state state;
	struct instruction instruction;
    uint16_t address;
    uint8_t cycle;

	// Peripherals
    bus_t* bus;
};

typedef struct CPU cpu_t;
typedef enum flag flag_t;
typedef enum address_mode address_mode_t;
typedef enum opcode opcode_t;
typedef struct instruction instruction_t;

// Branch instructions
void nesquick_cpu_bcc(cpu_t *cpu);
void nesquick_cpu_bcs(cpu_t *cpu);
void nesquick_cpu_beq(cpu_t *cpu);
void nesquick_cpu_bmi(cpu_t *cpu);
void nesquick_cpu_bne(cpu_t *cpu);
void nesquick_cpu_bpl(cpu_t *cpu);
void nesquick_cpu_bvc(cpu_t *cpu);
void nesquick_cpu_bvs(cpu_t *cpu);

// Flag instructions
void nesquick_cpu_clc(cpu_t *cpu);
void nesquick_cpu_cld(cpu_t *cpu);
void nesquick_cpu_cli(cpu_t *cpu);
void nesquick_cpu_clv(cpu_t *cpu);
void nesquick_cpu_sec(cpu_t *cpu);
void nesquick_cpu_sed(cpu_t *cpu);
void nesquick_cpu_sei(cpu_t *cpu);

// Load instructions
void nesquick_cpu_lda_abs(cpu_t *cpu);
void nesquick_cpu_lda_abs_x(cpu_t *cpu);
void nesquick_cpu_lda_abs_y(cpu_t *cpu);
void nesquick_cpu_lda_imm(cpu_t *cpu);
void nesquick_cpu_lda_ind_x(cpu_t *cpu);
void nesquick_cpu_lda_ind_y(cpu_t *cpu);
void nesquick_cpu_lda_zpg(cpu_t *cpu);
void nesquick_cpu_lda_zpg_x(cpu_t *cpu);

void nesquick_cpu_ldx_abs(cpu_t *cpu);
void nesquick_cpu_ldx_abs_y(cpu_t *cpu);
void nesquick_cpu_ldx_imm(cpu_t *cpu);
void nesquick_cpu_ldx_zpg(cpu_t *cpu);
void nesquick_cpu_ldx_zpg_y(cpu_t *cpu);

void nesquick_cpu_ldy_abs(cpu_t *cpu);
void nesquick_cpu_ldy_abs_x(cpu_t *cpu);
void nesquick_cpu_ldy_imm(cpu_t *cpu);
void nesquick_cpu_ldy_zpg(cpu_t *cpu);
void nesquick_cpu_ldy_zpg_x(cpu_t *cpu);

// Store instructions
void nesquick_cpu_sta_abs(cpu_t *cpu);
void nesquick_cpu_sta_abs_x(cpu_t *cpu);
void nesquick_cpu_sta_abs_y(cpu_t *cpu);
void nesquick_cpu_sta_ind_x(cpu_t *cpu);
void nesquick_cpu_sta_ind_y(cpu_t *cpu);
void nesquick_cpu_sta_zpg(cpu_t *cpu);
void nesquick_cpu_sta_zpg_x(cpu_t *cpu);

void nesquick_cpu_stx_abs(cpu_t *cpu);
void nesquick_cpu_stx_zpg(cpu_t *cpu);
void nesquick_cpu_stx_zpg_y(cpu_t *cpu);

void nesquick_cpu_sty_abs(cpu_t *cpu);
void nesquick_cpu_sty_zpg(cpu_t *cpu);
void nesquick_cpu_sty_zpg_x(cpu_t *cpu);

// Logical instructions
void nesquick_cpu_and(cpu_t *cpu, address_mode_t mode, uint16_t value);
void nesquick_cpu_eor(cpu_t *cpu, address_mode_t mode, uint16_t value);
void nesquick_cpu_ora(cpu_t *cpu, address_mode_t mode, uint16_t value);

void nesquick_cpu_adc(cpu_t *cpu, address_mode_t mode);

// Nesquick-specific functions
cpu_t *nesquick_cpu_init();
void nesquick_cpu_free(cpu_t *cpu);
void nesquick_cpu_print_debug(cpu_t* cpu);
void nesquick_cpu_print_ram(cpu_t* cpu);
void nesquick_cpu_reset(cpu_t* cpu);
void nesquick_cpu_tick(cpu_t* cpu);
void nesquick_cpu_execute_instruction(cpu_t *cpu);
bool nesquick_cpu_check_bit(uint8_t target, uint8_t value);
bool nesquick_cpu_check_flag(cpu_t *cpu, flag_t flag);
void nesquick_cpu_set_flag(cpu_t *cpu, flag_t flag);
void nesquick_cpu_clear_flag(cpu_t *cpu, flag_t flag);
instruction_t nesquick_cpu_decode_instruction(uint8_t i);