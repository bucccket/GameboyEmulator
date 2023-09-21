#include "window.h"

#include <stdint.h>

int WinInit(struct mfb_window* window) {
  if (!window) return WIN_OK;

  mfb_set_viewport(window, 50, 50, WIDTH - 50 - 50, HEIGHT - 50 - 50);

  return WIN_OK;
}

void ReadTile(uint8_t* ram, uint8_t offset, struct tile* tile) {
  uint8_t* pixelbuf = ram + offset;         // 8x8 area
  for (uint8_t u8 = 0; u8 < 16; u8 += 2) {  // iterate all rows
    uint8_t upper = pixelbuf[u8];
    uint8_t lower = pixelbuf[u8 + 1];
    for (uint8_t px = 0; px < 8; px++) {  // iterate each pixel in row
      tile->pixels[(u8 << 3) + px] = ((lower & (1 << px)) == (1 << px)) << 1;
      tile->pixels[(u8 << 3) + px] = ((upper & (1 << px)) == (1 << px));
    }
  }
}

int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram) {
  struct tile tileMapA[256];
  for (uint32_t i = 0; i < 256; i++) {
    struct tile tile = {.ID = i};
    ReadTile(ram, i << 4, &tile);
    tileMapA[i] = tile;
    // TODO: draw each tile
  }

  // framebuffer[i] = MFB_ARGB(0x5f, noise, noise, noise);

  mfb_update_state state = mfb_update_ex(window, framebuffer, WIDTH, HEIGHT);
  if (state != STATE_OK) {
    window = 0x0;
    return WIN_ERROR_CLOSE;
  }
  return WIN_OK;
}
