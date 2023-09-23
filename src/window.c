#include "window.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "minifb/MiniFB.h"

int WinInit(struct mfb_window* window) {
  if (!window) return WIN_OK;

  mfb_set_viewport(window, 0, 0, WIDTH << 2, HEIGHT << 2);

  return WIN_OK;
}

void ReadTile(uint8_t* tileram, struct tile* tile) {
  // assume palette is struct default for now
  for (int_fast8_t y = 0; y < 8; y++) {
    uint8_t lsb = *tileram++;
    uint8_t msb = *tileram++;
    for (int_fast8_t x = 0; x < 8; x++) {
      uint8_t pixel = CHECK_BIT(x, lsb) | CHECK_BIT(x, msb) << 1;
      tile->pixels[y][x] = tile->palette[pixel];
    }
  }
}

void PrintTile(struct tile tile) {
  for (int_fast8_t y = 0; y < 8; y++) {
    for (int_fast8_t x = 0; x < 8; x++) {
      switch (tile.pixels[y][x]) {
        case Black:
          printf(" ");
          break;
        case LGray:
          printf(":");
          break;
        case DGray:
          printf("v");
          break;
        case White:
          printf("@");
          break;
      }
    }
    printf("\n");
  }
}

void DrawTile(struct tile tile, uint32_t* framebuffer, uint8_t offx,
              uint8_t offy) {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      uint32_t pixel;
      switch (tile.pixels[y][x]) {
        case White:
          pixel = MFB_ARGB(0xFF, 224, 248, 208);
          break;
        case LGray:
          pixel = MFB_ARGB(0xFF, 136, 192, 112);
          break;
        case DGray:
          pixel = MFB_ARGB(0xFF, 52, 104, 86);
          break;
        case Black:
          pixel = MFB_ARGB(0xFF, 8, 24, 32);
          break;
      }
      framebuffer[((y + offy) << 8) + (x + offx)] = pixel;
    }
  }
}

int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram) {
  struct tile tileBankA[256];
  for (int_fast16_t tileidx = 0; tileidx < 256; tileidx++) {
    uint16_t offset = tileidx << 4;
    struct tile tile = {.ID = tileidx, .palette = {Black, LGray, DGray, White}};
    ReadTile(ram + 0x8000 + offset, &tile);
    tileBankA[tileidx] = tile;
    uint8_t x = tileidx % 16;
    uint8_t y = tileidx / 16;
    PrintTile(tileBankA[tileidx]);
    DrawTile(tileBankA[tileidx], framebuffer, x << 3, y << 3);
  }

  // framebuffer[i] = MFB_ARGB(0x5f, noise, noise, noise);

  mfb_update_state state = mfb_update_ex(window, framebuffer, WIDTH, HEIGHT);
  if (state != STATE_OK) {
    window = 0x0;
    return WIN_ERROR_CLOSE;
  }
  return WIN_OK;
}
