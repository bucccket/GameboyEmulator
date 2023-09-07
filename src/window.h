#pragma once

#include <MiniFB.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulator_types.h"

#define WIN_OK 0
#define WIN_ERROR_CLOSE 1

static uint32_t g_width = 160;
static uint32_t g_height = 144;

int WinInit(struct mfb_window* window);
int WinUpdate(struct mfb_window* window, BYTE* framebuffer);

/*-----+------------+
| 0b11 | white      |
| 0b10 | dark-gray  |
| 0b01 | light-gray |
| 0b00 | black      |
+------+-----------*/

/*            Bit Position
A            7 6 5 4 3 2 1 0
d          +-----------------+
d  0x8000  | 1 0 1 1 0 1 0 1 |
r          |-----------------|
e  0x8001  | 0 1 1 0 0 1 0 1 |
s          +-----------------+
s            D L W D B W B W
	       Color	    */
