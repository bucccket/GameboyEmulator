#include "window.h"

static uint32_t* g_buffer = 0x0;

int WinInit(struct mfb_window* window) {
  if (!window) return WIN_OK;

  g_buffer = (uint32_t*)malloc(g_width * g_height * 4);

  mfb_set_viewport(window, 50, 50, g_width - 50 - 50, g_height - 50 - 50);

  return WIN_OK;
}

int WinUpdate(struct mfb_window* window, uint8_t* framebuffer) {
  uint32_t i, noise, carry, seed = 0xbeef;

  for (i = 0; i < g_width * g_height; ++i) {
    noise = seed;
    noise >>= 3;
    noise ^= seed;
    carry = noise & 1;
    noise >>= 1;
    seed >>= 1;
    seed |= (carry << 30);
    noise &= 0xFF;
    g_buffer[i] = MFB_ARGB(0x5f, noise, noise, noise);
  }

  mfb_update_state state = mfb_update_ex(window, g_buffer, g_width, g_height);
  if (state != STATE_OK) {
    window = 0x0;
    return WIN_ERROR_CLOSE;
  }
  return WIN_OK;
}
