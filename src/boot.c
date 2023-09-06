#include "boot.h"

int BootLoadRom(BYTE* memory)
{
	const char* BootRomPath = "boot/DMG_ROM.bin";

	if (!memory) {
		printf("[ERROR] %s: no allocated memory for\n", __func__);
		return BOOT_ERROR_MEMORY;
	}

	FILE* file = fopen(BootRomPath, "rb");

	if (!file) {
		printf("[ERROR] %s: file %s not found!\n", __func__, BootRomPath);
		return BOOT_ERROR_FILE;
	}

	int readpos = 0;

	while (fread(&memory[readpos++], 1, 1, file))
		;

	fclose(file);

	return BOOT_OK;
}
