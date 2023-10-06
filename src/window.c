#include "window.h"

int WinInit(struct mfb_window* window, uint32_t width, uint32_t height) {
  if (!window) return WIN_OK;

  mfb_set_viewport(window, 0, 0, width << 1, height << 1);

  return WIN_OK;
}

void ReadTile(uint8_t* tileram, struct tile* tile) {
  // assume palette is struct default for now
  for (int_fast8_t y = 0; y < 8; y++) {
    uint8_t lsb = *tileram++;
    uint8_t msb = *tileram++;
    for (int_fast8_t x = 0; x < 8; x++) {
      uint8_t pixel = CHECK_BIT((7 - x), lsb) | CHECK_BIT((7 - x), msb) << 1;
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

// square_size uses bitshifts since all buffers are powers of 2
void DrawTile(struct tile tile, uint32_t* framebuffer, uint8_t offx,
              uint8_t offy, uint8_t square_size) {
  for (uint8_t y = 0; y < 8; y++) {
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
      framebuffer[((y + offy) << square_size) + (x + offx)] = pixel;
    }
  }
}

void DrawTileMap(struct tile* tileBank, uint32_t* framebuffer, uint8_t* ram) {
  uint16_t tilemap;
  if (CHECK_BIT(3, LCDC)) {
    tilemap = 0x9C00;
  } else {
    tilemap = 0x9800;
  }
  for (int_fast8_t tiley = 0; tiley < 32; tiley++) {
    for (int_fast8_t tilex = 0; tilex < 32; tilex++) {
      tiley %= 32;
      uint8_t tileidx = ram[tilemap + tilex + (tiley << 5)];
      DrawTile(tileBank[tileidx], framebuffer, tilex << 3, tiley << 3, 8);
    }
  }
}

void DrawTileData(struct tile* tileBank, uint32_t* framebuffer) {
  for (int_fast16_t tileidx = 0; tileidx < 256; tileidx++) {
    uint8_t x = tileidx % 16;
    uint8_t y = tileidx / 16;
    DrawTile(tileBank[tileidx], framebuffer, x << 3, y << 3, 7);
  }
}

int GraphicsUpdate(uint8_t cycles, uint8_t* ram) {
  static struct screen scanline;
  uint_fast16_t newX = scanline.posX + cycles;
  if (newX > 0xFF) {
    scanline.posX = (newX - 0xFF);
    scanline.posY++;
    ram[LY] = scanline.posY;  // line reset = overflow
  } else {
    scanline.posX = newX;
  }
  return WIN_OK;
}

int TileUpdate(struct mfb_window* window, uint32_t* tilebuffer, uint8_t* ram) {
  // Read LCDC

  uint16_t tiledata;  // might be a bug
  if (CHECK_BIT(4, LCDC)) {
    tiledata = 0x8800;
  } else {
    tiledata = 0x8000;
  }

  struct tile tileBank[256];
  for (int_fast16_t tileidx = 0; tileidx < 256; tileidx++) {
    uint16_t offset = tileidx << 4;
    struct tile tile = {.ID = tileidx, .palette = {Black, LGray, DGray, White}};
    ReadTile(ram + tiledata + offset, &tile);
    tileBank[tileidx] = tile;
  }

  if (tileBank[0].ID) {
    printf("wow");
  }
  DrawTileData(tileBank, tilebuffer);

  mfb_update_state state = mfb_update_ex(window, tilebuffer, 128, 128);
  if (state != STATE_OK) {
    window = 0x0;
    return WIN_ERROR_CLOSE;
  }
  return WIN_OK;
}

int WinUpdate(struct mfb_window* window, uint32_t* framebuffer, uint8_t* ram) {
  // Read LCDC
  uint16_t tiledata;  // might be a bug
  if (CHECK_BIT(4, LCDC)) {
    tiledata = 0x8800;
  } else {
    tiledata = 0x8000;
  }

  struct tile tileBank[256];
  for (int_fast16_t tileidx = 0; tileidx < 256; tileidx++) {
    uint16_t offset = tileidx << 4;
    struct tile tile = {.ID = tileidx, .palette = {Black, LGray, DGray, White}};
    ReadTile(ram + tiledata + offset, &tile);
    tileBank[tileidx] = tile;
  }

  // TODO: emulate hardware clock!

  // TODO: make internal framebuffer 8bit
  // TODO: make render region

  DrawTileMap(tileBank, framebuffer, ram);
  // DrawTileData(tileBank, framebuffer);

  uint32_t screen[DISPLAY_HEIGHT][DISPLAY_WIDTH];
  for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
      screen[y][x] = framebuffer[(x + ram[SCX]) + ((y + ram[SCY]) << 8)];
    }
  }

  // update graphics
  mfb_update_state state =
      mfb_update_ex(window, screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  if (state != STATE_OK) {
    window = 0x0;
    return WIN_ERROR_CLOSE;
  }
  return WIN_OK;
}
