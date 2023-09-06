#include "cpu.h"

int CpuStep(const BYTE* memory, BYTE* ram, unsigned short* pc, unsigned short* sp, struct Registers* reg, struct Flags* flag)
{
	printf("$%04X \t", *pc);
	BYTE opcode = memory[*pc];
	switch (opcode) {
	case 0x00: // NOP
		// done
		++*pc;
		break;
	case 0x06: // LD B,u8
		// op
		reg->B = memory[++*pc];
		// done
		++*pc;
		printf("[INSTR] LD B,$%02X\n", reg->B);
		break;
	case 0X05: // DEC B
		// flag
		flag->Z = 0;
		flag->N = 1;
		flag->H = 0;
		// op
		if (((reg->B & 0xF) + 1) & 0x10)
			flag->H = 1;
		reg->B++;
		if (!reg->B)
			flag->Z = 1;
		// done
		printf("[INSTR] DEC B\n");
		++*pc;
		break;
	case 0X0C: // INC C
		// op
		reg->C++;
		// done
		printf("[INSTR] INC C\n");
		++*pc;
		break;
	case 0x0E: // LD C,u8
		// op
		reg->C = memory[++*pc];
		// done
		printf("[INSTR] LD C,$%02X\n", memory[*pc]);
		++*pc;
		break;
	case 0x11: // LD DE,u16
		// op
		reg->E = memory[++*pc];
		reg->D = memory[++*pc];
		// done
		printf("[INSTR] LD DE,$%04X\n", DE);
		++*pc;
		break;
	case 0x13: // INC DE
	{
		// op
		unsigned short u16 = DE + 1;
		reg->D = u16 >> 010;
		reg->E = u16 & 0xFF;
		// done
		printf("[INSTR] INC DE\n");
		++*pc;
		break;
	}
	case 0x17: // RL A
		// flag
		flag->N = 0;
		flag->H = 0;
		flag->Z = 0;
		flag->C = reg->A & 0x80; // old bit #7
		// op
		reg->A = reg->A << 1 | (flag->C > 0);
		if (!reg->A)
			flag->Z = 1;
		// done
		printf("[INSTR] RL A\n");
		++*pc;
		break;
	case 0x1A: // LD A,(DE)
		// op
		reg->A = ram[DE];
		// done
		printf("[INSTR] LD A,(DE)\n");
		++*pc;
		break;
	case 0x20: // JR NZ,i8
	{
		// op
		char offset = (char)memory[++*pc];
		if (!flag->Z) {
			*pc += offset;
		}
		// done
		++*pc;
		printf("[INSTR] JR NZ,$%d pc=%04X %s\n", offset, *pc, !flag->Z ? "JMP" : "CONTINUE");
		if (*pc > 0x0010)
			getchar();
		break;
	}
	case 0x21: // LD HL,u16
		// op
		reg->L = memory[++*pc];
		reg->H = memory[++*pc];
		// done
		++*pc;
		printf("[INSTR] LD HL,$%04X\n", HL);
		break;
	case 0x22: // LD (HL+),A
	{
		// op
		ram[HL] = reg->A;
		unsigned short u16 = HL + 1;
		reg->H = u16 >> 010;
		reg->L = u16 & 0xFF;
		// done
		++*pc;
		printf("[INSTR] LD (HL+),A\n");
		break;
	}
	case 0x23: // INC HL
	{
		// op
		unsigned short u16 = HL + 1;
		reg->H = u16 >> 010;
		reg->L = u16 & 0xFF;
		// done
		printf("[INSTR] INC HL\n");
		++*pc;
		break;
	}
	case 0x31: // LD SP,u16
		// op
		*sp = memory[*pc + 1] | memory[*pc + 2] << 010;
		// done
		*pc += 3;
		printf("[INSTR] LD SP,$%04hX\n", *sp);
		break;
	case 0x32: // LD (HL-),A
	{
		// op
		ram[HL] = reg->A;
		unsigned short u16 = HL - 1;
		reg->H = u16 >> 010;
		reg->L = u16 & 0xFF;
		// done
		++*pc;
		printf("[INSTR] LD (HL-),A\n");
		break;
	}
	case 0x3E: // LD A,u8
		// op
		reg->A = memory[++*pc];
		// done
		printf("[INSTR] LD A,$%02X\n", memory[*pc]);
		++*pc;
		break;
	case 0x4F: // LD C,A
		// op
		reg->C = reg->A;
		// done
		printf("[INSTR] LD C,A\n");
		++*pc;
		break;
	case 0x77: // LD (HL),A
		// op
		ram[HL] = reg->A;
		// done
		++*pc;
		printf("[INSTR] LD (HL),A\n");
		break;
	case 0xAF: // XOR A,A
		// flag
		flag->N = 0;
		flag->H = 0;
		flag->C = 0;
		flag->Z = 0;
		//  op
		reg->A ^= reg->A;
		if (reg->A == 0)
			flag->Z = 1;
		// done
		++*pc;
		printf("[INSTR] XOR A,A -> A=%04X Z=%d\n", reg->A, flag->Z);
		break;
	case 0xBE: // CP A,(HL)
		// flag
		flag->N = 1;
		flag->C = 0;
		flag->Z = 0;
		// op
		BYTE u8 = ram[HL];
		if (reg->A == u8) {
			flag->Z = 1;
		} else if (reg->A < u8) {
			flag->C = 1;
		}
		// done
		printf("[INSTR] CP A, (HL)\n");
		++*pc;
		break;
	case 0xC1: // POP BC
		// op
		reg->B = ram[++*sp];
		reg->C = ram[++*sp];
		// done
		++*pc;
		printf("[INSTR] POP BC\n");
		break;
	case 0xC5: // PUSH BC
		// op
		ram[(*sp)--] = reg->B;
		ram[(*sp)--] = reg->C;
		// done
		++*pc;
		printf("[INSTR] PUSH BC\n");
		break;
	case 0xC9: // RET
		// op
		*pc = ram[*sp + 1] | ram[*sp + 2] << 010;
		*sp += 2;
		// done
		++*pc;
		printf("[INSTR] RET\n");
		break;
	case 0XCB: // PREFIX - extended opcodes
		opcode = memory[++*pc];
		switch (opcode) {
		case 0x11: // RL C
			// flag
			flag->N = 0;
			flag->H = 0;
			flag->Z = 0;
			flag->C = reg->C & 0x80; // old bit #7
			// op
			PrintBinary8(reg->C);
			reg->C = reg->C << 1 | (flag->C > 0);
			if (!reg->C)
				flag->Z = 1;
			PrintBinary8(reg->C);
			// done
			printf("[INSTR] RL C\n");
			++*pc;
			break;
		case 0x7C: // BIT 7,H
			// flag
			flag->N = 0;
			flag->H = 1;
			flag->Z = 0;
			// op
			if (!(reg->H & (1 << 7))) {
				flag->Z = 1;
			}
			// done
			++*pc;
			printf("[INSTR] BIT 7,H H=%02X Z=%d HL=%04X\n", reg->H, flag->Z, HL);
			break;
		default:
			printf("[ERROR] %s: Unkown extended instruction 0xCB 0x%02X at 0x%04hX\n", __func__, opcode, *pc);
			return CPU_ERROR_UNK_INSTRUCTION;
		}
		break;
	case 0xCD: // CALL u16
	{
		// op
		short address = memory[++*pc];
		address |= memory[++*pc] << 010;
		ram[(*sp)--] = *pc >> 010;
		ram[(*sp)--] = *pc & 0xFF;
		*pc = address;
		// done
		printf("[INSTR] CALL $%04X\n", *pc);
		getchar();
		break;
	}
	case 0xE0: // LD (FF00+u8),A
		// op
		ram[0xFF00 + memory[++*pc]] = reg->A;
		// done
		printf("[INSTR] LD ($FF00 + $%02X),A\n", memory[*pc]);
		++*pc;
		break;
	case 0xE2: // LD (FF00+C),A
		// op
		ram[0xFF00 + reg->C] = reg->A;
		// done
		++*pc;
		printf("[INSTR] LD (FF00+C),A\n");
		break;
	case 0xE5: // PUSH HL
		// op
		ram[(*sp)--] = reg->H;
		ram[(*sp)--] = reg->L;
		// done
		++*pc;
		printf("[INSTR] PUSH HL\n");
		break;
	default:
		printf("[ERROR] %s: Unkown instruction 0x%02X at 0x%04hX\n", __func__, opcode, *pc);
		return CPU_ERROR_UNK_INSTRUCTION;
	};

	return CPU_OK;
}

void PrintBinary8(BYTE u8)
{
	printf("%d", (u8 & (1 << 7)) != 0);
	printf("%d", (u8 & (1 << 6)) != 0);
	printf("%d", (u8 & (1 << 5)) != 0);
	printf("%d", (u8 & (1 << 4)) != 0);
	printf("%d", (u8 & (1 << 3)) != 0);
	printf("%d", (u8 & (1 << 2)) != 0);
	printf("%d", (u8 & (1 << 1)) != 0);
	printf("%d ", (u8 & (1 << 0)) != 0);
}
