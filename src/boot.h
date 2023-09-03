#pragma once

#include <stdio.h>

#include "emulator_types.h"

#define BOOT_OK 0
#define BOOT_ERROR_MEMORY 1
#define BOOT_ERROR_FILE 2

int BootLoadRom(BYTE* memory);
