#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"

int main() {
  printf("Launched\n");

  // ROM
  // uint8_t rom[0x100];
  // BootLoadRom(rom);

  // TEST
  uint8_t rom[0x10000];
  BootLoadTestRom(rom, "test/cpu_instrs.gb");

  // CPU
  uint8_t ram[0x10000];  // $0000-$FFFF
  memset(ram, 0x00, 0x10000);
  uint8_t cycles;

  unsigned short pc = 0x100, sp;
  struct Registers reg = {
      .a = 0, .b = 0, .c = 0, .d = 0, .e = 0, .f = 0, .h = 0, .l = 0};
  bool hlt = false;

  while (1) {
    if (!hlt) {
      if (CpuStep(rom, ram, &pc, &sp, &reg, &hlt, &cycles) != CPU_OK) break;
      DebugReadBlarggsSerial(ram);
    }
  }

  return 0;
}
