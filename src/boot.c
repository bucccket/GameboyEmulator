#include "boot.h"

int BootLoadRom(uint8_t rom[0x100]) {
  const char* BootRomPath = "boot/DMG_ROM.bin";  //"boot/DMG_ROM.bin";
  //
  FILE* file = fopen(BootRomPath, "rb");

  if (!rom) {
    printf("[ERROR] %s: no allocated rom for\n", __func__);
    return BOOT_ERROR_MEMORY;
  }

  if (!file) {
    printf("[ERROR] %s: file %s not found!\n", __func__, BootRomPath);
    return BOOT_ERROR_FILE;
  }

  int readpos = 0;

  while (fread(&rom[readpos++], 1, 1, file))
    ;

  fclose(file);

  return BOOT_OK;
}

int BootLoadTestRom(uint8_t** rom, const char* fileName) {
  const char* BootRomPath = fileName;
  FILE* file = fopen(BootRomPath, "rb");

  if (!file) {
    printf("[ERROR] %s: file %s not found!\n", __func__, BootRomPath);
    return BOOT_ERROR_FILE;
  }

  fseek(file, 0L, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  *rom = calloc(size, sizeof(uint8_t));

  if (!*rom) {
    printf("[ERROR] %s: no allocated rom for\n", __func__);
    return BOOT_ERROR_MEMORY;
  }

  fread(*rom, size, 1, file);
  fclose(file);

  return BOOT_OK;
}
