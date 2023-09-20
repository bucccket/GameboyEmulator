#pragma once

#include <stdint.h>
#include <stdio.h>

#define BOOT_OK 0
#define BOOT_ERROR_MEMORY 1
#define BOOT_ERROR_FILE 2

int BootLoadRom(uint8_t* memory);
int BootLoadTestRom(uint8_t* memory, const char* fileName);
