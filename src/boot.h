#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BOOT_OK 0
#define BOOT_ERROR_MEMORY 1
#define BOOT_ERROR_FILE 2

int BootLoadRom(uint8_t rom[0x100]);
int BootLoadTestRom(uint8_t** rom, const char* fileName);
