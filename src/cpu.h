#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "emulator_types.h"

#define CPU_OK 0
#define CPU_ERROR_UNK_INSTRUCTION 1

// combined registers - register name prefixed

#define AF ((short)reg->F | reg->A << 010)
#define BC ((short)reg->C | reg->B << 010)
#define DE ((short)reg->E | reg->D << 010)
#define HL ((short)reg->L | reg->H << 010)

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

int CpuStep(const BYTE* memory, BYTE* ram, unsigned short* pc, unsigned short* sp, struct Registers* reg, struct Flags* flags);
void PrintBinary8(BYTE u8);
