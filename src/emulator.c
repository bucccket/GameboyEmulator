#include <stdio.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"
#include "emulator_types.h"

int main()
{
	printf("Launched\n");

	BYTE memory[0x100];
	BootLoadRom(memory);

	struct timespec timer;
	// long nsecsA, nsecsB;

	short pc, sp;
	struct Registers reg = {
		.A = 0,
		.B = 0,
		.C = 0,
		.D = 0,
		.E = 0,
		.F = 0,
		.H = 0,
		.L = 0
	};

	struct Flags flags = {
		.Z = 0,
		.N = 0,
		.H = 0,
		.C = 0,
		.HLT = false
	};

	clock_gettime(CLOCK_REALTIME, &timer);
	while (1) {

		// nsecsA = timer.tv_nsec;
		clock_gettime(CLOCK_REALTIME, &timer);
		CpuStep(memory, &pc, &sp, &reg, &flags);
		// nsecsB = timer.tv_nsec;
		//  printf("diff: %ldns\n", nsecsB - nsecsA);
		clock_gettime(CLOCK_REALTIME, &timer);
	}

	return 0;
}
