#include "rom.h"

__extension__ void PrintRomType(uint8_t* ram) {
  uint8_t type = ram[0x0147];
  static const char* CartridgeTypes[0x100] = {
      "ROM ONLY",
      "ROM + MBC1",
      "ROM + MBC1 + RAM",
      "ROM + MBC1 + RAM + BATT",
      "UNKNOWN",
      "ROM + MBC2",
      "ROM + MBC2 + BATT",
      "UNKNOWN",
      "ROM + RAM",
      "ROM + RAM + BAT",
      "UNKNOWN",
      "ROM + MMM01",
      "ROM + MMM01 + SRAM",
      "ROM + MMM01 + SRAM + BATT",
      "UNKNOWN",
      "ROM + MBC3 + TIMER + BAT",
      "ROM + MBC3 + TIMER + RAM + BAT",
      "ROM + MBC3",
      "ROM + MBC3 + RAM",
      "ROM + MBC3 + RAM + BATT",
      [0x14 ... 0x18] = "UNKNOWN",
      "ROM + MBC5",
      "ROM + MBC5 + RAM",
      "ROM + MBC5 + RAM + BATT",
      "ROM + MBC5 + RUMBLE",
      "ROM + MBC5 + RUMBLE + SRAM",
      "ROM + MBC5 + RUMBLE + SRAM + BATT",
      "Pocket Camera",
      [0x20 ... 0xFC] = "UNKNOWN",
      "BANDAI TAMA5",
      "- Hudson HuC-3",
      "- Hudson HuC-1"};
  printf("[INFO] ROM TYPE: %s\n", CartridgeTypes[type]);
}

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wpedantic"

__extension__ void PrintRomSize(uint8_t* ram) {
  uint8_t type = ram[0x0148];
  uint32_t bytes;
  uint32_t banks;
  switch (type) {
    case 1 ... 6:
      bytes = 32768 << type;
      banks = 1 << type;
      break;
    case 0x52:
      bytes = 9437184;
      banks = 72;
      break;
    case 0x53:
      bytes = 10485760;
      banks = 80;
      break;
    case 0x54:
      bytes = 12582912;
      banks = 96;
      break;
  }
  printf("[INFO] ROM SIZE: %d KB %d banks\n", bytes, banks);
}
// #pragma GCC diagnostic pop

void PrintRamSize(uint8_t* ram) {
  uint8_t type = ram[0x0149];
  static const uint8_t bank_sizes[5] = {0, 1, 1, 4, 16};
  if (type) {
    uint32_t bytes = 512 << (type << 1);
    printf("[INFO] CATRIDGE RAM: %d KB %d banks\n", bytes, bank_sizes[type]);
  } else {
    printf("[INFO] CATRIDGE RAM: None\n");
  }
}
