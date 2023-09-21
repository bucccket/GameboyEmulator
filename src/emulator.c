#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"
#include "window.h"

int main() {
  printf("Launched\n");

  // ROM
  // uint8_t rom[0x100];
  // BootLoadRom(rom);

  static uint32_t framebuffer[256 * 256];  // Main Screen buffer @ 32x32 tiles
  struct mfb_window *window = mfb_open("Gameboy Emulator", WIDTH, HEIGHT);
  WinInit(window);

  // TEST
  uint8_t rom[0x10000];
  BootLoadTestRom(rom, "test/cpu_instrs.gb");

  // CPU
  uint8_t ram[0x10000];  // $0000-$FFFF
  memset(ram, 0x00, 0x10000);
  uint8_t cycles;
  bool IME = false;

  // INTERRUPTS
  ram[0xFFFF] = 0x00;  // IE
  ram[0xFF0F] = 0xe0;  // IF

  unsigned short pc = 0x100, sp = 0xFFFE;
  struct Registers reg = {
      .a = 0, .b = 0, .c = 0, .d = 0, .e = 0, .f = 0, .h = 0, .l = 0};
  bool hlt = false;

  while (1) {
    if (!hlt) {
      if (CpuStep(rom, ram, &pc, &sp, &reg, &hlt, &cycles, &IME) != CPU_OK)
        break;
      DebugReadBlarggsSerial(ram);
    }
    if (window) {
      WinUpdate(window, framebuffer, ram);
    }
  }

  return 0;
}
