#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minifb/MiniFB.h"

#define WIN_OK 0
#define WIN_ERROR_CLOSE 1

#define WIDTH 256
#define HEIGHT 256

// redef same impl
#ifndef CHECK_BIT
#define CHECK_BIT(b, r) ((r & (1 << b)) == (1 << b))
#endif

enum Colors {
  Black = 0,
  LGray = 1,
  DGray = 2,
  White = 3,
};

struct tile {
  int8_t ID;
  uint8_t pixels[8][8];  // 0-3 pixel color
  uint8_t palette[4];
};

int WinInit(struct mfb_window* window);
int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram);

void TileToPixels(struct tile, uint32_t* buffer);

/*-----+------------+
| 0b11 | white      | 224 248 208
| 0b10 | dark-gray  | 52  104 86
| 0b01 | light-gray | 136 192 112
| 0b00 | black      | 8   24  32
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
