//
// Created by sigma on 2/23/2020.
//

#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>

cpu_t *nesquick_cpu_init()
{
    cpu_t *cpu = (cpu_t*)malloc(sizeof(cpu_t));

	if(cpu != NULL)
	{
        cpu->A = 0;
        cpu->X = 0;
        cpu->Y = 0;
        cpu->P = 0;
        cpu->data = 0;
        cpu->PC = 0;
        cpu->SP = 0;
        cpu->debug = false;
        cpu->cycle = 0;
        cpu->address = 0;
	}

    return cpu;
}

void nesquick_cpu_free(cpu_t *cpu)
{
    free(cpu);
}

void nesquick_cpu_print_debug(cpu_t *cpu)
{
    printf("offset: %04x cycle: %02d a: %02x x: %02x y: %02x\n", cpu->PC, cpu->cycle, cpu->A, cpu->X, cpu->Y);
}

void nesquick_cpu_print_ram(cpu_t *cpu)
{
    const char* format = "%04x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n";
	
    for(int x = 0; x < cpu->bus->system_ram->size; x += 16)
    {
        uint8_t p1 = cpu->bus->system_ram->data[x];
        uint8_t p2 = cpu->bus->system_ram->data[x + 1];
        uint8_t p3 = cpu->bus->system_ram->data[x + 2];
        uint8_t p4 = cpu->bus->system_ram->data[x + 3];
        uint8_t p5 = cpu->bus->system_ram->data[x + 4];
        uint8_t p6 = cpu->bus->system_ram->data[x + 5];
        uint8_t p7 = cpu->bus->system_ram->data[x + 6];
        uint8_t p8 = cpu->bus->system_ram->data[x + 7];
        uint8_t p9 = cpu->bus->system_ram->data[x + 8];
        uint8_t p10 = cpu->bus->system_ram->data[x + 9];
        uint8_t p11 = cpu->bus->system_ram->data[x + 10];
        uint8_t p12 = cpu->bus->system_ram->data[x + 11];
        uint8_t p13 = cpu->bus->system_ram->data[x + 12];
        uint8_t p14 = cpu->bus->system_ram->data[x + 13];
        uint8_t p15 = cpu->bus->system_ram->data[x + 14];
        uint8_t p16 = cpu->bus->system_ram->data[x + 15];

        printf(format, x, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    }
}

void nesquick_cpu_reset(cpu_t *cpu)
{
	if(cpu->bus == NULL)
	{
        return;
	}
	
    cpu->PC = 0x0000;
    cpu->PC |= nesquick_bus_read(cpu->bus, 0xFFFC);
    cpu->PC |= nesquick_bus_read(cpu->bus, 0xFFFD) << 8;
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->P = 0;
    cpu->SP = 0;
    cpu->data = 0;
    cpu->address = 0;
    cpu->cycle = 0;
}

bool nesquick_cpu_check_bit(uint8_t target, uint8_t value)
{
    return (target & value) == value;
}

bool nesquick_cpu_check_flag(cpu_t* cpu, flag_t flag)
{
	switch(flag)
	{
    case FLAG_CARRY:
        return (cpu->P & FLAG_CARRY) == 1;
    case FLAG_ZERO:
        return (cpu->P & FLAG_ZERO) >> 1 == 1;
    case FLAG_INTERRUPT:
        return (cpu->P & FLAG_INTERRUPT) >> 2 == 1;
    case FLAG_DECIMAL:
        return (cpu->P & FLAG_DECIMAL) >> 3 == 1;
    case FLAG_OVERFLOW:
        return (cpu->P & FLAG_OVERFLOW) >> 6 == 1;
    case FLAG_NEGATIVE:
        return (cpu->P & FLAG_NEGATIVE) >> 7 == 1;
    default:
        return false;
	}
}

void nesquick_cpu_set_flag(cpu_t *cpu, flag_t flag)
{
	switch(flag)
	{
    case FLAG_CARRY:
        cpu->P |= FLAG_CARRY;
        return;
    case FLAG_ZERO:
        cpu->P |= FLAG_ZERO;
        return;
    case FLAG_INTERRUPT:
        cpu->P |= FLAG_INTERRUPT;
        return;
    case FLAG_DECIMAL:
        cpu->P |= FLAG_DECIMAL;
        return;
    case FLAG_OVERFLOW:
        cpu->P |= FLAG_OVERFLOW;
        return;
    case FLAG_NEGATIVE:
        cpu->P |= FLAG_NEGATIVE;
        return;
    default:
        return;
	}
}

void nesquick_cpu_clear_flag(cpu_t *cpu, flag_t flag)
{
	switch(flag)
	{
    case FLAG_CARRY:
		if((cpu->P & FLAG_CARRY) == 1)
		{
            cpu->P = ~(FLAG_CARRY);
		}
        return;
    case FLAG_ZERO:
        if ((cpu->P & FLAG_ZERO) >> 1 == 1)
        {
            cpu->P = ~(FLAG_ZERO);
        }
        return;
    case FLAG_INTERRUPT:
        if ((cpu->P & FLAG_INTERRUPT) >> 2 == 1)
        {
            cpu->P = ~(FLAG_INTERRUPT);
        }
        return;
    case FLAG_DECIMAL:
        if ((cpu->P & FLAG_DECIMAL) >> 3 == 1)
        {
            cpu->P = ~(FLAG_DECIMAL);
        }
        return;
    case FLAG_OVERFLOW:
        if ((cpu->P & FLAG_OVERFLOW) >> 6 == 1)
        {
            cpu->P = ~(FLAG_OVERFLOW);
        }
        return;
    case FLAG_NEGATIVE:
        if ((cpu->P & FLAG_NEGATIVE) >> 7 == 1)
        {
            cpu->P = ~(FLAG_NEGATIVE);
        }
        return;
    default:
        return;
	}
}

void nesquick_cpu_execute_instruction(cpu_t *cpu)
{
    uint8_t i = nesquick_bus_read(cpu->bus, cpu->PC);
    cpu->instruction = nesquick_cpu_decode_instruction(i);
	
	switch(cpu->instruction.operation)
	{
    case OPCODE_BCC:
        nesquick_cpu_bcc(cpu);
        break;
    case OPCODE_BCS:
        nesquick_cpu_bcs(cpu);
        break;
    case OPCODE_BEQ:
        nesquick_cpu_beq(cpu);
        break;
    case OPCODE_BMI:
        nesquick_cpu_bmi(cpu);
        break;
    case OPCODE_BNE:
        nesquick_cpu_bne(cpu);
        break;
    case OPCODE_BPL:
        nesquick_cpu_bpl(cpu);
        break;
    case OPCODE_BVC:
        nesquick_cpu_bvc(cpu);
        break;
    case OPCODE_BVS:
        nesquick_cpu_bvs(cpu);
        break;
    case OPCODE_LDA:
		switch(cpu->instruction.mode)
		{
        case MODE_ABSOLUTE:
            nesquick_cpu_lda_abs(cpu);
            break;
        case MODE_ABSOLUTE_X:
            nesquick_cpu_lda_abs_x(cpu);
            break;
        case MODE_ABSOLUTE_Y:
            nesquick_cpu_lda_abs_y(cpu);
            break;
        case MODE_IMMEDIATE:
            nesquick_cpu_lda_imm(cpu);
            break;
        case MODE_INDIRECT_X:
            nesquick_cpu_lda_ind_x(cpu);
            break;
        case MODE_INDIRECT_Y:
            nesquick_cpu_lda_ind_y(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_lda_zpg(cpu);
            break;
        case MODE_ZEROPAGE_X:
            nesquick_cpu_lda_zpg_x(cpu);
            break;
		}
        break;
    case OPCODE_LDX:
		switch(cpu->instruction.mode)
		{
        case MODE_ABSOLUTE:
            nesquick_cpu_ldx_abs(cpu);
            break;
        case MODE_ABSOLUTE_Y:
            nesquick_cpu_ldx_abs_y(cpu);
            break;
        case MODE_IMMEDIATE:
            nesquick_cpu_ldx_imm(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_ldx_zpg(cpu);
            break;
        case MODE_ZEROPAGE_Y:
            nesquick_cpu_ldx_zpg_y(cpu);
            break;
		}
        break;
    case OPCODE_LDY:
		switch(cpu->instruction.mode)
		{
        case MODE_ABSOLUTE:
            nesquick_cpu_ldy_abs(cpu);
            break;
        case MODE_ABSOLUTE_X:
            nesquick_cpu_ldy_abs_x(cpu);
            break;
        case MODE_IMMEDIATE:
            nesquick_cpu_ldy_imm(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_ldy_zpg(cpu);
            break;
        case MODE_ZEROPAGE_X:
            nesquick_cpu_ldy_zpg_x(cpu);
            break;
		}
        break;
    case OPCODE_STA:
        switch (cpu->instruction.mode)
        {
        case MODE_ABSOLUTE:
            nesquick_cpu_sta_abs(cpu);
            break;
        case MODE_ABSOLUTE_X:
            nesquick_cpu_sta_abs_x(cpu);
            break;
        case MODE_ABSOLUTE_Y:
            nesquick_cpu_sta_abs_y(cpu);
            break;
        case MODE_INDIRECT_X:
            nesquick_cpu_sta_ind_x(cpu);
            break;
        case MODE_INDIRECT_Y:
            nesquick_cpu_sta_ind_y(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_sta_zpg(cpu);
            break;
        case MODE_ZEROPAGE_X:
            nesquick_cpu_sta_zpg_x(cpu);
            break;
        }
        break;
    case OPCODE_STX:
		switch(cpu->instruction.mode)
		{
        case MODE_ABSOLUTE:
            nesquick_cpu_stx_abs(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_stx_zpg(cpu);
            break;
        case MODE_ZEROPAGE_Y:
            nesquick_cpu_stx_zpg_y(cpu);
            break;
		}
        break;
    case OPCODE_STY:
        switch (cpu->instruction.mode)
        {
        case MODE_ABSOLUTE:
            nesquick_cpu_sty_abs(cpu);
            break;
        case MODE_ZEROPAGE:
            nesquick_cpu_sty_zpg(cpu);
            break;
        case MODE_ZEROPAGE_X:
            nesquick_cpu_sty_zpg_x(cpu);
            break;
        }
        break;
	}
}

instruction_t nesquick_cpu_decode_instruction(uint8_t i)
{
    //const uint8_t i = nesquick_bus_read(cpu->bus, cpu->PC);

	switch(i)
	{
    case 0x00:
        return (instruction_t) { OPCODE_BRK, MODE_IMPLIED };
    case 0x01:
        return (instruction_t) { OPCODE_ORA, MODE_INDIRECT_X };
    case 0x05:
        return (instruction_t) { OPCODE_ORA, MODE_ZEROPAGE };
    case 0x06:
        return (instruction_t) { OPCODE_ASL, MODE_ZEROPAGE };
    case 0x08:
        return (instruction_t) { OPCODE_PHP, MODE_IMPLIED };
    case 0x09:
        return (instruction_t) { OPCODE_ORA, MODE_IMMEDIATE };
    case 0x0A:
        return (instruction_t) { OPCODE_ASL, MODE_ACCUMULATOR };
    case 0x0D:
        return (instruction_t) { OPCODE_ORA, MODE_ABSOLUTE };
    case 0x0E:
        return (instruction_t) { OPCODE_ASL, MODE_ABSOLUTE };
    case 0x10:
        return (instruction_t) { OPCODE_BPL, MODE_RELATIVE };
    case 0x11:
        return (instruction_t) { OPCODE_ORA, MODE_INDIRECT_Y };
    case 0x15:
        return (instruction_t) { OPCODE_ORA, MODE_ZEROPAGE_X };
    case 0x16:
        return (instruction_t) { OPCODE_ASL, MODE_ZEROPAGE_X };
    case 0x18:
        return (instruction_t) { OPCODE_CLC, MODE_IMPLIED };
    case 0x19:
        return (instruction_t) { OPCODE_ORA, MODE_ABSOLUTE_Y };
    case 0x1D:
        return (instruction_t) { OPCODE_ORA, MODE_ABSOLUTE_X };
    case 0x1E:
        return (instruction_t) { OPCODE_ASL, MODE_ABSOLUTE_X };
    case 0x20:
        return (instruction_t) { OPCODE_JMP, MODE_ABSOLUTE };
    case 0x21:
        return (instruction_t) { OPCODE_AND, MODE_INDIRECT_X };
    case 0x24:
        return (instruction_t) { OPCODE_BIT, MODE_ZEROPAGE };
    case 0x25:
        return (instruction_t) { OPCODE_AND, MODE_ZEROPAGE };
    case 0x26:
        return (instruction_t) { OPCODE_ROL, MODE_ZEROPAGE };
    case 0x28:
        return (instruction_t) { OPCODE_PLP, MODE_IMPLIED };
    case 0x29:
        return (instruction_t) { OPCODE_AND, MODE_IMMEDIATE };
    case 0x2A:
        return (instruction_t) { OPCODE_ROL, MODE_ACCUMULATOR };
    case 0x2C:
        return (instruction_t) { OPCODE_BIT, MODE_ABSOLUTE };
    case 0x2D:
        return (instruction_t) { OPCODE_AND, MODE_ABSOLUTE };
    case 0x2E:
        return (instruction_t) { OPCODE_ROL, MODE_ABSOLUTE_X };
    case 0x30:
        return (instruction_t) { OPCODE_BMI, MODE_RELATIVE };
    case 0x31:
        return (instruction_t) { OPCODE_AND, MODE_INDIRECT_Y };
    case 0x35:
        return (instruction_t) { OPCODE_AND, MODE_ZEROPAGE_X };
    case 0x36:
        return (instruction_t) { OPCODE_ROL, MODE_ZEROPAGE_X };
    case 0x38:
        return (instruction_t) { OPCODE_SEC, MODE_IMPLIED };
    case 0x39:
        return (instruction_t) { OPCODE_AND, MODE_ABSOLUTE_Y };
    case 0x3D:
        return (instruction_t) { OPCODE_AND, MODE_ABSOLUTE_X };
    case 0x3E:
        return (instruction_t) { OPCODE_ROL, MODE_ABSOLUTE_X };
    case 0x40:
        return (instruction_t) { OPCODE_RTI, MODE_IMPLIED };
    case 0x45:
        return (instruction_t) { OPCODE_EOR, MODE_ZEROPAGE };
    case 0x46:
        return (instruction_t) { OPCODE_LSR, MODE_ZEROPAGE };
    case 0x48:
        return (instruction_t) { OPCODE_PHA, MODE_IMPLIED };
    case 0x49:
        return (instruction_t) { OPCODE_EOR, MODE_IMMEDIATE };
    case 0x4A:
        return (instruction_t) { OPCODE_LSR, MODE_ACCUMULATOR };
    case 0x4C:
        return (instruction_t) { OPCODE_JMP, MODE_ABSOLUTE };
    case 0x4D:
        return (instruction_t) { OPCODE_EOR, MODE_ABSOLUTE };
    case 0x4E:
        return (instruction_t) { OPCODE_LSR, MODE_ABSOLUTE };
    case 0x50:
        return (instruction_t) { OPCODE_BVC, MODE_RELATIVE };
    case 0x51:
        return (instruction_t) { OPCODE_EOR, MODE_INDIRECT_Y };
    case 0x55:
        return (instruction_t) { OPCODE_EOR, MODE_ZEROPAGE_X };
	case 0x56:
        return (instruction_t) { OPCODE_LSR, MODE_ZEROPAGE_X };
    case 0x58:
        return (instruction_t) { OPCODE_CLI, MODE_IMPLIED };
	case 0x59:
        return (instruction_t) { OPCODE_EOR, MODE_ABSOLUTE_Y };
    case 0x5D:
        return (instruction_t) { OPCODE_ADC, MODE_ABSOLUTE };
    case 0x5E:
        return (instruction_t) { OPCODE_LSR, MODE_ABSOLUTE_X };
    case 0x60:
        return (instruction_t) { OPCODE_RTS, MODE_IMPLIED };
    case 0x61:
        return (instruction_t) { OPCODE_ADC, MODE_INDIRECT_X };
    case 0x65:
        return (instruction_t) { OPCODE_ADC, MODE_ZEROPAGE };
    case 0x66:
        return (instruction_t) { OPCODE_ROR, MODE_ZEROPAGE };
    case 0x68:
        return (instruction_t) { OPCODE_PLA, MODE_IMPLIED };
    case 0x69:
        return (instruction_t) { OPCODE_ADC, MODE_IMMEDIATE };
    case 0x6A:
        return (instruction_t) { OPCODE_ROR, MODE_ACCUMULATOR };
    case 0x6C:
        return (instruction_t) { OPCODE_JMP, MODE_INDIRECT };
    case 0x6D:
        return (instruction_t) { OPCODE_ADC, MODE_ABSOLUTE };
    case 0x6E:
        return (instruction_t) { OPCODE_ROR, MODE_ABSOLUTE };
    case 0x70:
        return (instruction_t) { OPCODE_BVS, MODE_RELATIVE };
    case 0x71:
        return (instruction_t) { OPCODE_ADC, MODE_INDIRECT_X };
    case 0x75:
        return (instruction_t) { OPCODE_ADC, MODE_ZEROPAGE_X };
    case 0x76:
        return (instruction_t) { OPCODE_ROR, MODE_ZEROPAGE_X };
    case 0x78:
        return (instruction_t) { OPCODE_SEI, MODE_IMPLIED };
    case 0x79:
        return (instruction_t) { OPCODE_ADC, MODE_ABSOLUTE_Y };
    case 0x7D:
        return (instruction_t) { OPCODE_ADC, MODE_ABSOLUTE_X };
    case 0x7E:
        return (instruction_t) { OPCODE_ROR, MODE_ABSOLUTE_X };
    case 0x81:
        return (instruction_t) { OPCODE_STA, MODE_INDIRECT_X };
    case 0x84:
        return (instruction_t) { OPCODE_STY, MODE_ZEROPAGE };
    case 0x85:
        return (instruction_t) { OPCODE_STA, MODE_ZEROPAGE };
    case 0x86:
        return (instruction_t) { OPCODE_STX, MODE_ZEROPAGE };
    case 0x88:
        return (instruction_t) { OPCODE_DEY, MODE_IMPLIED };
    case 0x8A:
        return (instruction_t) { OPCODE_TXA, MODE_IMPLIED };
    case 0x8C:
        return (instruction_t) { OPCODE_STY, MODE_ABSOLUTE };
    case 0x8D:
        return (instruction_t) { OPCODE_STA, MODE_ABSOLUTE };
    case 0x8E:
        return (instruction_t) { OPCODE_STX, MODE_ABSOLUTE };
    case 0x90:
        return (instruction_t) { OPCODE_BCC, MODE_RELATIVE };
    case 0x91:
        return (instruction_t) { OPCODE_STA, MODE_INDIRECT_Y };
    case 0x94:
        return (instruction_t) { OPCODE_STY, MODE_ZEROPAGE_X };
    case 0x95:
        return (instruction_t) { OPCODE_STA, MODE_ZEROPAGE_X };
    case 0x96:
        return (instruction_t) { OPCODE_STX, MODE_ZEROPAGE_Y };
    case 0x98:
        return (instruction_t) { OPCODE_TYA, MODE_IMPLIED };
    case 0x99:
        return (instruction_t) { OPCODE_STA, MODE_ABSOLUTE_Y };
    case 0x9A:
        return (instruction_t) { OPCODE_TXS, MODE_IMPLIED };
    case 0x9D:
        return (instruction_t) { OPCODE_STA, MODE_ABSOLUTE_X };
    case 0xA0:
        return (instruction_t) { OPCODE_LDY, MODE_IMMEDIATE };
    case 0xA1:
        return (instruction_t) { OPCODE_LDA, MODE_INDIRECT_X };
    case 0xA2:
        return (instruction_t) { OPCODE_LDX, MODE_IMMEDIATE };
    case 0xA4:
        return (instruction_t) { OPCODE_LDY, MODE_ZEROPAGE };
    case 0xA5:
        return (instruction_t) { OPCODE_LDA, MODE_ZEROPAGE };
    case 0xA6:
        return (instruction_t) { OPCODE_LDX, MODE_ZEROPAGE };
    case 0xA8:
        return (instruction_t) { OPCODE_TAY, MODE_IMPLIED };
    case 0xA9:
        return (instruction_t) { OPCODE_LDA, MODE_IMMEDIATE };
    case 0xAA:
        return (instruction_t) { OPCODE_TAX, MODE_IMPLIED };
    case 0xAC:
        return (instruction_t) { OPCODE_LDY, MODE_ABSOLUTE };
    case 0xAD:
        return (instruction_t) { OPCODE_LDA, MODE_ABSOLUTE };
    case 0xAE:
        return (instruction_t) { OPCODE_LDX, MODE_ABSOLUTE };
    case 0xB0:
        return (instruction_t) { OPCODE_BCS, MODE_RELATIVE };
    case 0xB1:
        return (instruction_t) { OPCODE_LDA, MODE_INDIRECT_Y };
    case 0xB4:
        return (instruction_t) { OPCODE_LDY, MODE_ZEROPAGE_X };
    case 0xB5:
        return (instruction_t) { OPCODE_LDA, MODE_ZEROPAGE_X };
    case 0xB6:
        return (instruction_t) { OPCODE_LDX, MODE_ZEROPAGE_Y };
    case 0xB8:
        return (instruction_t) { OPCODE_CLV, MODE_IMPLIED };
    case 0xB9:
        return (instruction_t) { OPCODE_LDA, MODE_ABSOLUTE_Y };
    case 0xBA:
        return (instruction_t) { OPCODE_TSX, MODE_IMPLIED };
    case 0xBC:
        return (instruction_t) { OPCODE_LDY, MODE_ABSOLUTE_X };
    case 0xBD:
        return (instruction_t) { OPCODE_LDA, MODE_ABSOLUTE_X };
    case 0xBE:
        return (instruction_t) { OPCODE_LDX, MODE_ABSOLUTE_Y };
    case 0xC0:
        return (instruction_t) { OPCODE_CPY, MODE_IMMEDIATE };
    case 0xC1:
        return (instruction_t) { OPCODE_CMP, MODE_INDIRECT_X };
    case 0xC4:
        return (instruction_t) { OPCODE_CPY, MODE_ZEROPAGE };
    case 0xC5:
        return (instruction_t) { OPCODE_CMP, MODE_ZEROPAGE };
    case 0xC6:
        return (instruction_t) { OPCODE_DEC, MODE_ZEROPAGE };
    case 0xC8:
        return (instruction_t) { OPCODE_INY, MODE_IMPLIED };
    case 0xC9:
        return (instruction_t) { OPCODE_CMP, MODE_IMMEDIATE };
    case 0xCA:
        return (instruction_t) { OPCODE_DEX, MODE_IMPLIED };
    case 0xCC:
        return (instruction_t) { OPCODE_CPY, MODE_ABSOLUTE };
    case 0xCD:
        return (instruction_t) { OPCODE_CMP, MODE_ABSOLUTE };
    case 0xCE:
        return (instruction_t) { OPCODE_DEC, MODE_ABSOLUTE };
    case 0xD0:
        return (instruction_t) { OPCODE_BNE, MODE_RELATIVE };
    case 0xD1:
        return (instruction_t) { OPCODE_CMP, MODE_INDIRECT_Y };
    case 0xD5:
        return (instruction_t) { OPCODE_CMP, MODE_ZEROPAGE_X };
    case 0xD6:
        return (instruction_t) { OPCODE_DEC, MODE_ZEROPAGE_X };
    case 0xD8:
        return (instruction_t) { OPCODE_CLD, MODE_IMPLIED };
    case 0xD9:
        return (instruction_t) { OPCODE_CMP, MODE_ABSOLUTE_Y };
    case 0xDD:
        return (instruction_t) { OPCODE_CMP, MODE_ABSOLUTE_X };
    case 0xDE:
        return (instruction_t) { OPCODE_DEC, MODE_ABSOLUTE_X };
    case 0xE0:
        return (instruction_t) { OPCODE_CPX, MODE_IMMEDIATE };
    case 0xE1:
        return (instruction_t) { OPCODE_SBC, MODE_INDIRECT_X };
    case 0xE4:
        return (instruction_t) { OPCODE_CPX, MODE_ZEROPAGE };
    case 0xE5:
        return (instruction_t) { OPCODE_SBC, MODE_ZEROPAGE };
    case 0xE6:
        return (instruction_t) { OPCODE_INC, MODE_ZEROPAGE };
    case 0xE8:
        return (instruction_t) { OPCODE_INX, MODE_IMPLIED };
    case 0xE9:
        return (instruction_t) { OPCODE_SBC, MODE_IMMEDIATE };
    case 0xEA:
        return (instruction_t) { OPCODE_NOP, MODE_IMPLIED };
    case 0xEC:
        return (instruction_t) { OPCODE_CPX, MODE_ABSOLUTE };
    case 0xED:
        return (instruction_t) { OPCODE_SBC, MODE_ABSOLUTE };
    case 0xEE:
        return (instruction_t) { OPCODE_INC, MODE_ABSOLUTE };
    case 0xF0:
        return (instruction_t) { OPCODE_BEQ, MODE_RELATIVE };
    case 0xF1:
        return (instruction_t) { OPCODE_SBC, MODE_INDIRECT_Y };
    case 0xF5:
        return (instruction_t) { OPCODE_SBC, MODE_ZEROPAGE_X };
    case 0xF6:
        return (instruction_t) { OPCODE_INC, MODE_ZEROPAGE_X };
    case 0xF8:
        return (instruction_t) { OPCODE_SED, MODE_IMPLIED };
    case 0xF9:
        return (instruction_t) { OPCODE_SBC, MODE_ABSOLUTE_Y };
    case 0xFD:
        return (instruction_t) { OPCODE_SBC, MODE_ABSOLUTE_X };
    case 0xFE:
        return (instruction_t) { OPCODE_INC, MODE_ABSOLUTE_X };
    default:
        return (instruction_t) { OPCODE_ILLEGAL, MODE_IMPLIED };
	}
}
