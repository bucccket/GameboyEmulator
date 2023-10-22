#define _POSIX_C_SOURCE 200809L

#include <MiniFB.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "boot.h"
#include "cpu.h"
#include "rom.h"
#include "window.h"

static volatile int keepRunning = 1;

static uint8_t* rom;
static uint8_t ram[0x10000];  // $0000-$FFFF
void coreDumpHandle(int dummy) {
  CoreDump("core-GameboyEmulator.dmp", ram);
  free(rom);
  exit(0);
}

int main() {
  printf("Launched\n");

  signal(SIGINT, coreDumpHandle);

  // ROM
  uint8_t boot[0x100];
  BootLoadRom(boot);

  // TEST
  uint8_t rom_reboot_vec[0x100];
  BootLoadTestRom(&rom, "roms/cpu_instrs.gb");
  // BootLoadTestRom(&rom, "roms/Tetris (World) (Rev A).gb");
  // BootLoadTestRom(&rom, "roms/Dr. Mario (World).gb");
  // BootLoadTestRom(&rom, "roms/Link's Awakening.gb");
  // SWAP
  memcpy(rom_reboot_vec, rom, 0x100);

  // WINDOW
  static uint32_t framebuffer[256 * 256];  // Main Screen buffer @ 32x32 tiles
  struct mfb_window* window =
      mfb_open("Gameboy Emulator", DISPLAY_WIDTH << 1, DISPLAY_HEIGHT << 1);
  WinInit(window, DISPLAY_WIDTH << 1, DISPLAY_HEIGHT << 1);

  // TILEWINDOW
  static uint32_t tilebuffer[128 * 128];
  struct mfb_window* tilewindow =
      mfb_open("Gameboy Emulator", 128 << 1, 128 << 1);
  WinInit(window, 128 << 1, 128 << 1);

  // CPU
  memset(ram, 0x00, 0x10000);
  memcpy(ram, rom, 0x4000);  // copy BNK0
  memcpy(ram, boot, 0x100);  // insert BIOS
  uint8_t cycles;
  bool IME = false;

  // GB HEADER
  // Cartridge Type:
  PrintRomType(ram);
  PrintRomSize(ram);
  PrintRamSize(ram);

  // INTERRUPTS
  ram[0xFFFF] = 0x00;  // IE
  ram[0xFF0F] = 0xe0;  // IF

  unsigned short pc = 0x0, sp = 0xFFFE;
  struct Registers reg = {.a = 0x00,
                          .b = 0x00,
                          .c = 0x00,
                          .d = 0x00,
                          .e = 0x00,
                          .f = 0x00,
                          .h = 0x00,
                          .l = 0x00};
  bool hlt = false;

  struct timespec timerA, timerB;

  // SETUP
  while (1) {
    const int MAXCYCLES = 69905;
    int cyclesThisUpdate = 0;
    while (cyclesThisUpdate < MAXCYCLES) {
      // start timer
      clock_gettime(CLOCK_REALTIME, &timerA);
      // ROM SWAP
      // TODO: Evaluate old value and update with ram read inside if!
      if (ram[0xFF50]) {
        memcpy(ram, rom_reboot_vec, 0x100);
        memcpy(rom, rom_reboot_vec, 0x100);
        ram[0xFF50] = 0;
      }
      // CPU
      if (!hlt) {
        if (CpuStep(ram, &pc, &sp, &reg, &hlt, &cycles, &IME) != CPU_OK) break;
        DebugReadBlarggsSerial(ram);
      }
      // UPDATE
      cyclesThisUpdate += cycles;
      // PPU
      GraphicsUpdate(cycles, ram);
    }
    if (timerB.tv_nsec < timerA.tv_nsec) {
      timerB.tv_nsec += 1e9;
    }

    if (window) WinUpdate(window, framebuffer, ram);
    if (window)
      if (!mfb_wait_sync(window)) window = 0x0;
    if (tilewindow) TileUpdate(tilewindow, tilebuffer, ram);
    if (tilewindow)
      if (!mfb_wait_sync(tilewindow)) tilewindow = 0x0;
    // TIMER SYNC
    /*
    clock_gettime(CLOCK_REALTIME, &timerB);
    if (timerB.tv_nsec < timerA.tv_nsec) {
      timerB.tv_nsec += 1e9;
    }
    uint64_t timediff = (timerB.tv_nsec - timerA.tv_nsec) - 16666666;
    if ((-timediff) > 100000)
      nanosleep(&(struct timespec){.tv_nsec = (-timediff - 50000)}, NULL);
    */
  }

  return 0;
}
