#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CPU_OK 0
#define CPU_ERROR_UNK_INSTRUCTION 1
#define CPU_ERROR_FAULT 2

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) \
  do {                   \
    printf(__VA_ARGS__); \
  } while (0)
#else
#define DEBUG_PRINT(...) \
  do {                   \
  } while (0)
#endif

#define A (reg->a)
#define F (reg->f)
#define B (reg->b)
#define C (reg->c)
#define D (reg->d)
#define E (reg->e)
#define H (reg->h)
#define L (reg->l)

#define AF ((uint16_t)F | A << 010)
#define BC ((uint16_t)C | B << 010)
#define DE ((uint16_t)E | D << 010)
#define HL ((uint16_t)L | H << 010)

#define SET_Z (F |= 0x80)
#define SET_N (F |= 0x40)
#define SET_H (F |= 0x20)
#define SET_C (F |= 0x10)

#define RES_Z (F &= ~(0x80))
#define RES_N (F &= ~(0x40))
#define RES_H (F &= ~(0x20))
#define RES_C (F &= ~(0x10))

#define GET_Z ((F & 0x80) == 0x80)
#define GET_N ((F & 0x40) == 0x40)
#define GET_H ((F & 0x20) == 0x20)
#define GET_C ((F & 0x10) == 0x10)

#define IF_Z(cond) ((cond) ? SET_Z : RES_Z)
#define IF_N(cond) ((cond) ? SET_N : RES_N)
#define IF_H(cond) ((cond) ? SET_H : RES_H)
#define IF_C(cond) ((cond) ? SET_C : RES_C)

#define CHECK_BIT(b, r) ((r & (1 << b)) == (1 << b))
#define SET_BIT(b, r) (r |= (1 << b))
#define RES_BIT(b, r) (r &= (~(1 << b)))

#define HALFCARRY_8(n, m) (((((n) & (0xF)) + ((m) & (0xF))) & 0x10) == 0x10)

#define CARRY_8(n, m) (((uint16_t)(n + m) & 0x100) == 0x100)

#define HALFCARRY_16(n, m) \
  (((((n) & (0xFF)) + ((m) & (0xFF))) & 0x1000) == 0x1000)

#define CARRY_16(n, m) (((uint32_t)(n + m) & 0x10000) == 0x10000)

struct Registers {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  uint8_t d;
  uint8_t e;
  uint8_t f;
  uint8_t h;
  uint8_t l;
};

int CpuStep(const uint8_t* rom, uint8_t* ram, unsigned short* pc,
            unsigned short* sp, struct Registers* reg, bool* hlt,
            uint8_t* cycles, bool* IME);
void DebugReadBlarggsSerial(uint8_t* ram);
void PrintBinary8(uint8_t u8);
void CoreDump(const char* fileName, uint8_t* ram);
