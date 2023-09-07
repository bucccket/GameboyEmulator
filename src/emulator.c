#include <stdio.h>
#include <string.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"
#include "emulator_types.h"
// #include "window.h"

int main()
{
	printf("Launched\n");

	BYTE memory[0x100];
	BootLoadRom(memory);

	// struct mfb_window* window = mfb_open("Noise Test", g_width, g_height);
	//  WinInit(window);

	struct timespec timer;
	// long nsecsA, nsecsB;

	BYTE ram[0x10000]; // $0000-$FFFF
	memset(ram, 0x00, 0x10000);

	unsigned short pc, sp;
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

	struct Flags flag = {
		.Z = 0,
		.N = 0,
		.H = 0,
		.C = 0,
		.HLT = false
	};

	clock_gettime(CLOCK_REALTIME, &timer);
	while (!flag.HLT) {

		// nsecsA = timer.tv_nsec;
		clock_gettime(CLOCK_REALTIME, &timer);
		if (CpuStep(memory, ram, &pc, &sp, &reg, &flag) != CPU_OK)
			break;
		// if (WinUpdate(window, (BYTE*)(ram + 0x8000)) || !mfb_wait_sync(window))
		// break;
		// nsecsB = timer.tv_nsec;
		//  printf("diff: %ldns\n", nsecsB - nsecsA);
		clock_gettime(CLOCK_REALTIME, &timer);
	}

	return 0;
}
