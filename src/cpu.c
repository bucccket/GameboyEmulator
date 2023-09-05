#include "cpu.h"

int CpuStep(const BYTE* memory, BYTE* ram, unsigned short* pc, unsigned short* sp, struct Registers* reg, struct Flags* flag)
{
	BYTE opcode = memory[*pc];
	switch (opcode) {
	case 0x00: // NOP
		// done
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
	case 0x20: // JR NZ,i8
	{
		// op
		char offset = (char)memory[++*pc];
		if (!flag->Z) {
			*pc += offset;
		}
		// done
		++*pc;
		printf("[INSTR] JZ NZ,$%d pc=%04X %s\n", offset, *pc + 1, !flag->Z ? "JMP" : "CONTINUE");
		break;
	}
	case 0x21: // LD HL,u16
		// op
		reg->L = memory[++*pc];
		reg->H = memory[++*pc];
		// done
		++*pc;
		printf("[INSTR] LD HL,$%04X -> sp=$%04X\n", HL, *sp);
		break;
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
	case 0XCB: // PREFIX - extended opcodes
		opcode = memory[++*pc];
		switch (opcode) {
		case 0x7C: // BIT 7,H
			// flag
			flag->N = 0;
			flag->H = 1;
			flag->Z = 0;
			// op
			if (!(reg->H & (1 << 7))) {
				flag->Z = 1;
			}
			if (HL == 0x7FFD)
				getchar();
			// done
			++*pc;
			printf("[INSTR] BIT 7,H H=%02X Z=%d HL=%04X\n", reg->H, flag->Z, HL);
			break;
		default:
			printf("[ERROR] %s: Unkown extended instruction 0xCB 0x%02X at 0x%04hX\n", __func__, opcode, *pc);
			return CPU_ERROR_UNK_INSTRUCTION;
		}
		break;
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
	case 0xE5: // PUSH DL
		// op

		// done
		printf("[INSTR] LD (FF00+C),A\n");
		break;
	default:
		printf("[ERROR] %s: Unkown instruction 0x%02X at 0x%04hX\n", __func__, opcode, *pc);
		return CPU_ERROR_UNK_INSTRUCTION;
	};

	return CPU_OK;
}
