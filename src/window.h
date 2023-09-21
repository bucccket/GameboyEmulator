#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "minifb/MiniFB.h"

#define WIN_OK 0
#define WIN_ERROR_CLOSE 1

#define WIDTH 160
#define HEIGHT 144

struct tile {
  int8_t ID;
  uint8_t pixels[16 * 16];  // 0-3 pixel color
  // TODO: palette;
};

int WinInit(struct mfb_window* window);
int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram);

void TileToPixels(struct tile, uint32_t* buffer);

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
