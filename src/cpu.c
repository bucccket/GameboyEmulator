#include "cpu.h"

int CpuStep(BYTE* memory, short* pc, short* sp, struct Registers* reg, struct Flags* flags)
{

	switch (memory[*pc]) {
	// NOOP
	case 0x00:
		++*pc;
		break;
	default:
		printf("[ERROR] %s: Unkown instruction %02X\n", __func__, memory[*pc]);
		return CPU_ERROR_UNK_INSTRUCTION;
	};

	return CPU_OK;
}
