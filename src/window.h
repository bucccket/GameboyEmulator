#pragma once

#include <MiniFB.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN_OK 0
#define WIN_ERROR_CLOSE 1

// tile buffer
#define WIDTH 256
#define HEIGHT 256

// GB screen buffer
#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

// IO regs
#define LCDC 0xFF40
/*-LCDC-+----------------------------+----- 0 ----+--------- 1 ---------+
 |  7   | R/W LCD Enable             | No picture | opertation          |
 |  6   | R/W Window Tile Map Sel    | 9800-9BFF  | 9C00-9FFF           |
 |  5   | R/W Window Display         | off        | on                  |
 |  4   | R/W BG & Win Tile Data Sel | 8800-97FF  | 8000-8FFF (overlap) |
 |  3   | R/W BG Tile Map Select     | 9800-9BFF  | 9C00-9FFF           |
 |  2   | R/W OBJ (Sprite) Size      | 8 x 8      | 8 x 16 (w x h)      |
 |  1   | R/W OBJ (Sprite) Display   | off        | on                  |
 |  0   | R/W BG & Window Display    | off        | on                  |
 +------+----------------------------+------------+--------------------*/

#define STAT 0xFF41
/*-STAT-+---------------------+-----------------------------------------+
 |  6   | Int Status for LCDC | LYC == LY (Selectable)                  |
 |  5   | R/W                 | Mode 10                                 |
 |  4   |                     | Mode 01                                 |
 |  3   |                     | Mode 00                                 |
 |  2   |    Coincidence Flag | LYC == (LCDC) LY                        |
 | 1-0  | R  Mode Flag        | 00: During V-Blank                      |
 |      |                     | 01: During H-Blank                      |
 |      |                     | 10: During Searching OAM-RAM            |
 |      |                     | 11: During Transferring Data to LCD Drv |
 +------+---------------------+----------------------------------------*/

#define SCY 0xFF42
#define SCX 0xFF43
#define LY 0xFF44
#define LYC 0xFF45
#define DMA 0xFF46
#define BGP 0xFF47
/*-BGP-+------------+
 | 7-6 | Black 0b00 |
 | 5-4 | Dark  0b10 |
 | 3-2 | Light 0b01 |
 | 1-0 | White 0b11 |
 +-----+-----------*/

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
  uint8_t pixels[8][8];
  uint8_t palette[4];
};

struct screen {
  uint8_t posX;
  uint8_t posY;
};

int WinInit(struct mfb_window* window, uint32_t width, uint32_t height);
int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram);
int TileUpdate(struct mfb_window* window, uint32_t* tilebuffer, uint8_t* ram);
int GraphicsUpdate(uint8_t cycles, uint8_t* ram);

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
