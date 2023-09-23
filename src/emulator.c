#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"
#include "minifb/MiniFB.h"
#include "window.h"

int main() {
  printf("Launched\n");

  // ROM
  uint8_t boot[0x100];
  BootLoadRom(boot);

  // TEST
  uint8_t rom[0x10000];
  BootLoadTestRom(rom, "test/cpu_instrs.gb");

  memcpy(rom, boot, 0x100);

  // WINDOW
  static uint32_t framebuffer[256 * 256];  // Main Screen buffer @ 32x32 tiles
  struct mfb_window *window =
      mfb_open("Gameboy Emulator", WIDTH << 2, HEIGHT << 2);
  WinInit(window);

  // CPU
  uint8_t ram[0x10000];  // $0000-$FFFF
  memset(ram, 0x00, 0x10000);
  memcpy(ram, rom, 0x8000);
  uint8_t cycles;
  bool IME = false;

  // INTERRUPTS
  ram[0xFFFF] = 0x00;  // IE
  ram[0xFF0F] = 0xe0;  // IF

  unsigned short pc = 0x00, sp = 0xFFFE;
  struct Registers reg = {.a = 0x00,
                          .b = 0x00,
                          .c = 0x00,
                          .d = 0x00,
                          .e = 0x00,
                          .f = 0x00,
                          .h = 0x00,
                          .l = 0x00};
  bool hlt = false;

  while (1) {
    if (!hlt) {
      if (CpuStep(rom, ram, &pc, &sp, &reg, &hlt, &cycles, &IME) != CPU_OK)
        break;
      DebugReadBlarggsSerial(ram);
    }
    if (window && pc == 60) {
      WinUpdate(window, framebuffer, ram);
    }
  }

  return 0;
}
