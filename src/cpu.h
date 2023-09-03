#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "emulator_types.h"

#define CPU_OK 0
#define CPU_ERROR_UNK_INSTRUCTION 1

struct Registers {
	BYTE A;
	BYTE B;
	BYTE C;
	BYTE D;
	BYTE E;
	BYTE F;
	BYTE H;
	BYTE L;
};

struct Flags {
	BYTE Z;
	BYTE N;
	BYTE H;
	BYTE C;
	bool HLT;
};

int CpuStep(BYTE* memory, short* pc, short* sp, struct Registers* reg, struct Flags* flags);
