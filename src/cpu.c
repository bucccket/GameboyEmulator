#include "cpu.h"

#include <stdint.h>
#include <stdio.h>

#include "window.h"

// R  - 8 bit Register
// RR - 16 bit Register
// (--) - dereference at address
// u16 - read 16 bit from ram
// u8 - read 8 bit from ram
// 00h - hexadecimal number literal

int CpuStep(const uint8_t *memory, uint8_t *ram, uint16_t *pc, uint16_t *sp,
            struct Registers *reg, bool *hlt, uint8_t *cycles, bool *IME) {
  static struct debug dbg = {.trace = DBG_CONTINUE};

  uint8_t opcode = ram[*pc];
  if (opcode) DEBUG_PRINT(MAG "$%04X:%02X \t" RESET, *pc, opcode);
  if (*sp == 0x0) {
    printf("[ERROR] SP underflowing\n");
    return CPU_ERROR_FAULT;
  }
  switch (opcode) {
    case 0x00:  // NOP
      // DEBUG_PRINT("[INSTR] NOP\n");
      *cycles = 4;
      ++*pc;
      return CPU_OK;
      break;

    /*--------------
     *  8-Bit Loads
     *-------------*/
    case 0x06:  // LD B, u8
      B = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD B, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;
    case 0x0E:  // LD C, u8
      C = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD C, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;
    case 0x16:  // LD D, u8
      D = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD D, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;
    case 0x1E:  // LD E, u8
      E = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD E, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;
    case 0x26:  // LD H, u8
      H = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD H, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;
    case 0x2E:  // LD L, u8
      L = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD L, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;

    case 0x7F:  // LD A, A
      A = A;
      DEBUG_PRINT("[INSTR] LD A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x78:  // LD A, B
      A = B;
      DEBUG_PRINT("[INSTR] LD A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x79:  // LD A, C
      A = C;
      DEBUG_PRINT("[INSTR] LD A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x7A:  // LD A, D
      A = D;
      DEBUG_PRINT("[INSTR] LD A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x7B:  // LD A, E
      A = E;
      DEBUG_PRINT("[INSTR] LD A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x7C:  // LD A, H
      A = H;
      DEBUG_PRINT("[INSTR] LD A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x7D:  // LD A, L
      A = L;
      DEBUG_PRINT("[INSTR] LD A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x0A:  // LD A, (BC)
      A = ram[BC];
      DEBUG_PRINT("[INSTR] LD A, (BC)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x1A:  // LD A, (DE)
      A = ram[DE];
      DEBUG_PRINT("[INSTR] LD A, (DE)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x7E:  // LD A, (HL)
      A = ram[HL];
      DEBUG_PRINT("[INSTR] LD A, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xFA:  // LD A, (u16)
    {
      uint16_t u16 = ram[++*pc] | ram[++*pc] << 010;
      A = ram[u16];
      DEBUG_PRINT("[INSTR] LD A, ($%04X)\n", u16);
      ++*pc;
      *cycles = 16;
      break;
    }
    case 0x3E:  // LD A, u8
      A = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD A, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;

    case 0x40:  // LD B, B
      B = B;
      DEBUG_PRINT("[INSTR] LD B, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x41:  // LD B, C
      B = C;
      DEBUG_PRINT("[INSTR] LD B, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x42:  // LD B, D
      B = D;
      DEBUG_PRINT("[INSTR] LD B, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x43:  // LD B, E
      B = E;
      DEBUG_PRINT("[INSTR] LD B, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x44:  // LD B, H
      B = H;
      DEBUG_PRINT("[INSTR] LD B, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x45:  // LD B, L
      B = L;
      DEBUG_PRINT("[INSTR] LD B, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x46:  // LD B, (HL)
      B = ram[HL];
      DEBUG_PRINT("[INSTR] LD B, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x48:  // LD C, B
      C = B;
      DEBUG_PRINT("[INSTR] LD C, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x49:  // LD C, C
      C = C;
      DEBUG_PRINT("[INSTR] LD C, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4A:  // LD C, D
      C = D;
      DEBUG_PRINT("[INSTR] LD C, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4B:  // LD C, E
      C = E;
      DEBUG_PRINT("[INSTR] LD C, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4C:  // LD C, H
      C = H;
      DEBUG_PRINT("[INSTR] LD C, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4D:  // LD C, L
      C = L;
      DEBUG_PRINT("[INSTR] LD C, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4E:  // LD C, (HL)
      C = ram[HL];
      DEBUG_PRINT("[INSTR] LD C, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x50:  // LD D, B
      D = B;
      DEBUG_PRINT("[INSTR] LD D, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x51:  // LD D, C
      D = C;
      DEBUG_PRINT("[INSTR] LD D, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x52:  // LD D, D
      D = D;
      DEBUG_PRINT("[INSTR] LD D, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x53:  // LD D, E
      D = E;
      DEBUG_PRINT("[INSTR] LD D, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x54:  // LD D, H
      D = H;
      DEBUG_PRINT("[INSTR] LD D, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x55:  // LD D, L
      D = L;
      DEBUG_PRINT("[INSTR] LD D, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x56:  // LD D, (HL)
      D = ram[HL];
      DEBUG_PRINT("[INSTR] LD D, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x58:  // LD E, B
      E = B;
      DEBUG_PRINT("[INSTR] LD E, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x59:  // LD E, C
      E = C;
      DEBUG_PRINT("[INSTR] LD E, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5A:  // LD E, D
      E = D;
      DEBUG_PRINT("[INSTR] LD E, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5B:  // LD E, E
      E = E;
      DEBUG_PRINT("[INSTR] LD E, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5C:  // LD E, H
      E = H;
      DEBUG_PRINT("[INSTR] LD E, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5D:  // LD E, L
      E = L;
      DEBUG_PRINT("[INSTR] LD E, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5E:  // LD E, (HL)
      E = ram[HL];
      DEBUG_PRINT("[INSTR] LD E, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x60:  // LD H, B
      H = B;
      DEBUG_PRINT("[INSTR] LD H, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x61:  // LD H, C
      H = C;
      DEBUG_PRINT("[INSTR] LD H, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x62:  // LD H, D
      H = D;
      DEBUG_PRINT("[INSTR] LD H, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x63:  // LD H, E
      H = E;
      DEBUG_PRINT("[INSTR] LD H, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x64:  // LD H, H
      H = H;
      DEBUG_PRINT("[INSTR] LD H, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x65:  // LD H, L
      H = L;
      DEBUG_PRINT("[INSTR] LD H, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x66:  // LD H, (HL)
      H = ram[HL];
      DEBUG_PRINT("[INSTR] LD H, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x68:  // LD L, B
      L = B;
      DEBUG_PRINT("[INSTR] LD L, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x69:  // LD L, C
      L = C;
      DEBUG_PRINT("[INSTR] LD L, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6A:  // LD L, D
      L = D;
      DEBUG_PRINT("[INSTR] LD L, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6B:  // LD L, E
      L = E;
      DEBUG_PRINT("[INSTR] LD L, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6C:  // LD L, H
      L = H;
      DEBUG_PRINT("[INSTR] LD L, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6D:  // LD L, L
      L = L;
      DEBUG_PRINT("[INSTR] LD L, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6E:  // LD L, (HL)
      L = ram[HL];
      DEBUG_PRINT("[INSTR] LD L, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x70:  // LD (HL), B
      ram[HL] = B;
      DEBUG_PRINT("[INSTR] LD (HL), B\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x71:  // LD (HL), C
      ram[HL] = C;
      DEBUG_PRINT("[INSTR] LD (HL), C\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x72:  // LD (HL), D
      ram[HL] = D;
      DEBUG_PRINT("[INSTR] LD (HL), D\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x73:  // LD (HL), E
      ram[HL] = E;
      DEBUG_PRINT("[INSTR] LD (HL), E\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x74:  // LD (HL), H
      ram[HL] = H;
      DEBUG_PRINT("[INSTR] LD (HL), H\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x75:  // LD (HL), L
      ram[HL] = L;
      DEBUG_PRINT("[INSTR] LD (HL), L\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x36:  // LD (HL), u8
      ram[HL] = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD (HL), $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 12;
      break;

    case 0x47:  // LD B, A
      B = A;
      DEBUG_PRINT("[INSTR] LD B, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x4F:  // LD C, A
      C = A;
      DEBUG_PRINT("[INSTR] LD C, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x57:  // LD D, A
      D = A;
      DEBUG_PRINT("[INSTR] LD D, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x5F:  // LD E, A
      E = A;
      DEBUG_PRINT("[INSTR] LD E, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x67:  // LD H, A
      H = A;
      DEBUG_PRINT("[INSTR] LD H, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x6F:  // LD L, A
      L = A;
      DEBUG_PRINT("[INSTR] LD L, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x02:  // LD (BC), A
      ram[BC] = A;
      DEBUG_PRINT("[INSTR] LD (BC), A\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x12:  // LD (DE), A
      ram[DE] = A;
      DEBUG_PRINT("[INSTR] LD (DE), A\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0x77:  // LD (HL), A
      ram[HL] = A;
      DEBUG_PRINT("[INSTR] LD (HL), A\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xEA:  // LD (u16), A
    {
      uint16_t u16 = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 2;
      ram[u16] = A;
      DEBUG_PRINT("[INSTR] LD ($%04X), A\n", u16);
      ++*pc;
      *cycles = 16;
      break;
    }

    case 0xF2:  // LD A, (FF00 + C)
      A = ram[0xFF00 + C];
      DEBUG_PRINT("[INSTR] LD A, (FF00 + C)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xE2:  // LD (FF00 + C), A
      ram[0xFF00 + C] = A;
      DEBUG_PRINT("[INSTR] LD (FF00 + C), A\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x3A:  // LD A, (HL-)
    {
      A = ram[HL];
      uint16_t u16 = HL - 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] LD A, (HL-)\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x32:  // LD (HL-), A
    {
      ram[HL] = A;
      uint16_t u16 = HL - 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] LD (HL-), A\n");
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0x2A:  // LD A, (HL+)
    {
      A = ram[HL];
      uint16_t u16 = HL + 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] LD A, (HL+)\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x22:  // LD (HL+), A
    {
      ram[HL] = A;
      uint16_t u16 = HL + 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] LD (HL+), A\n");
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0xF0:  // LD A, (FF00 + u8)
      A = ram[0xFF00 + ram[++*pc]];
      DEBUG_PRINT("[INSTR] LD A, (FF00 + $%02X)\n", ram[*pc]);
      ++*pc;
      *cycles = 12;
      break;
    case 0xE0:  // LD (FF00 + u8), A
      ram[0xFF00 + ram[++*pc]] = A;
      DEBUG_PRINT("[INSTR] LD (FF00 + $%02X), A\n", ram[*pc]);
      ++*pc;
      *cycles = 12;
      break;

    /*---------------
     *  16-Bit Loads
     *--------------*/
    case 0x01:  // LD BC, u16
      C = ram[++*pc];
      B = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD BC, $%04X\n", BC);
      ++*pc;
      *cycles = 12;
      break;
    case 0x11:  // LD DE, u16
      E = ram[++*pc];
      D = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD DE, $%04X\n", DE);
      ++*pc;
      *cycles = 12;
      break;
    case 0x21:  // LD HL, u16
      L = ram[++*pc];
      H = ram[++*pc];
      DEBUG_PRINT("[INSTR] LD HL, $%04X\n", HL);
      ++*pc;
      *cycles = 12;
      break;
    case 0x31:  // LD SP, u16
      *sp = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 2;
      DEBUG_PRINT("[INSTR] LD SP, $%04X\n", *sp);
      ++*pc;
      *cycles = 12;
      break;

    case 0xF9:  // LD SP, HL
      *sp = HL;
      DEBUG_PRINT("[INSTR] LD SP, $%04X\n", *sp);
      ++*pc;
      *cycles = 8;
      break;

    case 0xF8:  // LD HL, SP+i8
    {
      RES_Z;
      RES_N;
      int8_t i8 = (int8_t)ram[++*pc];
      uint16_t u16 = *sp + i8;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] LD HL, $%04X\n", HL);
      IF_H(HALFCARRY_16(*sp, i8));
      IF_C(CARRY_16(*sp, i8));
      ++*pc;
      *cycles = 12;
      break;
    }

    case 0x08:  // LD (u16), HL
      ram[ram[++*pc]] = L;
      ram[ram[++*pc]] = H;
      DEBUG_PRINT("[INSTR] LD ($%04X), HL\n", *((uint16_t *)ram - 2));
      ++*pc;
      *cycles = 20;
      break;

    case 0xF5:  //  PUSH AF
      ram[--*sp] = A;
      ram[--*sp] = F;
      DEBUG_PRINT("[INSTR] PUSH AF\n");
      ++*pc;
      *cycles = 16;
      break;
    case 0xC5:  //  PUSH BC
      ram[--*sp] = B;
      ram[--*sp] = C;
      DEBUG_PRINT("[INSTR] PUSH BC\n");
      ++*pc;
      *cycles = 16;
      break;
    case 0xD5:  //  PUSH DE
      ram[--*sp] = D;
      ram[--*sp] = E;
      DEBUG_PRINT("[INSTR] PUSH DE\n");
      ++*pc;
      *cycles = 16;
      break;
    case 0xE5:  //  PUSH HL
      ram[--*sp] = H;
      ram[--*sp] = L;
      DEBUG_PRINT("[INSTR] PUSH HL\n");
      ++*pc;
      *cycles = 16;
      break;

    case 0xF1:  //  POP AF
      F = ram[(*sp)++];
      A = ram[(*sp)++];
      DEBUG_PRINT("[INSTR] POP AF\n");
      ++*pc;
      *cycles = 12;
      break;
    case 0xC1:  //  POP BC
      C = ram[(*sp)++];
      B = ram[(*sp)++];
      DEBUG_PRINT("[INSTR] POP BC\n");
      ++*pc;
      *cycles = 12;
      break;
    case 0xD1:  //  POP DE
      E = ram[(*sp)++];
      D = ram[(*sp)++];
      DEBUG_PRINT("[INSTR] POP DE\n");
      ++*pc;
      *cycles = 12;
      break;
    case 0xE1:  //  POP HL
      L = ram[(*sp)++];
      H = ram[(*sp)++];
      DEBUG_PRINT("[INSTR] POP HL\n");
      ++*pc;
      *cycles = 12;
      break;

    /*------------
     *  8-Bit ALU
     *-----------*/
    case 0x87:  //  ADD A, A
    {
      RES_N;
      A += A;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, A));
      IF_C(CARRY_8(A, A));
      DEBUG_PRINT("[INSTR] ADD A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x80:  //  ADD A, B
    {
      RES_N;
      A += B;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, B));
      IF_C(CARRY_8(A, B));
      DEBUG_PRINT("[INSTR] ADD A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x81:  //  ADD A, C
    {
      RES_N;
      A += C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, C));
      IF_C(CARRY_8(A, C));
      DEBUG_PRINT("[INSTR] ADD A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x82:  //  ADD A, D
    {
      RES_N;
      A += D;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, D));
      IF_C(CARRY_8(A, D));
      DEBUG_PRINT("[INSTR] ADD A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x83:  //  ADD A, E
    {
      RES_N;
      A += E;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, E));
      IF_C(CARRY_8(A, E));
      DEBUG_PRINT("[INSTR] ADD A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x84:  //  ADD A, H
    {
      RES_N;
      A += H;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, H));
      IF_C(CARRY_8(A, H));
      DEBUG_PRINT("[INSTR] ADD A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x85:  //  ADD A, L
    {
      RES_N;
      A += L;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, L));
      IF_C(CARRY_8(A, L));
      DEBUG_PRINT("[INSTR] ADD A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x86:  //  ADD A, (HL)
    {
      RES_N;
      uint8_t u8 = ram[HL];
      A += u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, u8));
      IF_C(CARRY_8(A, u8));
      DEBUG_PRINT("[INSTR] ADD A, (HL)X\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0xC6:  //  ADD A, u8
    {
      RES_N;
      uint8_t u8 = ram[++*pc];
      A += u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, u8));
      IF_C(CARRY_8(A, u8));
      DEBUG_PRINT("[INSTR] ADD A, $%02X\n", u8);
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0x8F:  //  ADC A, A
    {
      RES_N;
      A += A + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, A + GET_C));
      IF_C(CARRY_8(A, A + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x88:  //  ADC A, B
    {
      RES_N;
      A += B + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, B + GET_C));
      IF_C(CARRY_8(A, B + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x89:  //  ADC A, C
    {
      RES_N;
      A += C + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, C + GET_C));
      IF_C(CARRY_8(A, C + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x8A:  //  ADC A, D
    {
      RES_N;
      A += D + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, D + GET_C));
      IF_C(CARRY_8(A, D + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x8B:  //  ADC A, E
    {
      RES_N;
      A += E + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, E + GET_C));
      IF_C(CARRY_8(A, E + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x8C:  //  ADC A, H
    {
      RES_N;
      A += H + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, H + GET_C));
      IF_C(CARRY_8(A, H + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x8D:  //  ADC A, L
    {
      RES_N;
      A += L + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, L + GET_C));
      IF_C(CARRY_8(A, L + GET_C));
      DEBUG_PRINT("[INSTR] ADC A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x8E:  //  ADC A, (HL)
    {
      RES_N;
      uint8_t u8 = ram[HL] + GET_C;
      A += u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, u8));
      IF_C(CARRY_8(A, u8));
      DEBUG_PRINT("[INSTR] ADC A, (HL)X\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0xCE:  //  ADC A, u8
    {
      RES_N;
      uint8_t u8 = ram[++*pc] + GET_C;
      A += u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, u8));
      IF_C(CARRY_8(A, u8));
      DEBUG_PRINT("[INSTR] ADC A, $%02X\n", u8);
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0x97:  //  SUB A, A
    {
      SET_N;
      A -= A;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(A + 1)));
      IF_C(CARRY_8(A, ~(A + 1)));
      DEBUG_PRINT("[INSTR] SUB A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x90:  //  SUB A, B
    {
      SET_N;
      A -= B;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(B + 1)));
      IF_C(CARRY_8(A, ~(B + 1)));
      DEBUG_PRINT("[INSTR] SUB A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x91:  //  SUB A, C
    {
      SET_N;
      A -= C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(C + 1)));
      IF_C(CARRY_8(A, ~(C + 1)));
      DEBUG_PRINT("[INSTR] SUB A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x92:  //  SUB A, D
    {
      SET_N;
      A -= D;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(D + 1)));
      IF_C(CARRY_8(A, ~(D + 1)));
      DEBUG_PRINT("[INSTR] SUB A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x93:  //  SUB A, E
    {
      SET_N;
      A -= E;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(E + 1)));
      IF_C(CARRY_8(A, ~(E + 1)));
      DEBUG_PRINT("[INSTR] SUB A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x94:  //  SUB A, H
    {
      SET_N;
      A -= H;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(H + 1)));
      IF_C(CARRY_8(A, ~(H + 1)));
      DEBUG_PRINT("[INSTR] SUB A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x95:  //  SUB A, L
    {
      SET_N;
      A -= L;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(L + 1)));
      IF_C(CARRY_8(A, ~(L + 1)));
      DEBUG_PRINT("[INSTR] SUB A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x96:  //  SUB A, (HL)
    {
      SET_N;
      uint8_t u8 = ram[HL];
      A -= u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(u8 + 1)));
      IF_C(CARRY_8(A, ~(u8 + 1)));
      DEBUG_PRINT("[INSTR] SUB A, (HL)X\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0xD6:  //  SUB A, u8
    {
      SET_N;
      uint8_t u8 = ram[++*pc];
      A -= u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(u8 + 1)));
      IF_C(CARRY_8(A, ~(u8 + 1)));
      DEBUG_PRINT("[INSTR] SUB A, $%02X\n", u8);
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0x9F:  //  SBC A, A
    {
      SET_N;
      A -= A + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(A + 1) + GET_C));
      IF_C(CARRY_8(A, ~(A + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x98:  //  SBC A, B
    {
      SET_N;
      A -= B + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(B + 1) + GET_C));
      IF_C(CARRY_8(A, ~(B + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x99:  //  SBC A, C
    {
      SET_N;
      A -= C + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(C + 1) + GET_C));
      IF_C(CARRY_8(A, ~(C + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x9A:  //  SBC A, D
    {
      SET_N;
      A -= D + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(D + 1) + GET_C));
      IF_C(CARRY_8(A, ~(D + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x9B:  //  SBC A, E
    {
      SET_N;
      A -= E + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(E + 1) + GET_C));
      IF_C(CARRY_8(A, ~(E + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x9C:  //  SBC A, H
    {
      SET_N;
      A -= H + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(H + 1) + GET_C));
      IF_C(CARRY_8(A, ~(H + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x9D:  //  SBC A, L
    {
      SET_N;
      A -= L + GET_C;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(L + 1) + GET_C));
      IF_C(CARRY_8(A, ~(L + 1) + GET_C));
      DEBUG_PRINT("[INSTR] SBC A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x9E:  //  SBC A, (HL)
    {
      SET_N;
      uint8_t u8 = ram[HL] + GET_C;
      A -= u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(u8 + 1)));
      IF_C(CARRY_8(A, ~(u8 + 1)));
      DEBUG_PRINT("[INSTR] SBC A, (HL)X\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0xDE:  //  SBC A, u8
    {
      SET_N;
      uint8_t u8 = ram[++*pc] + GET_C;
      A -= u8;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, ~(u8 + 1)));
      IF_C(CARRY_8(A, ~(u8 + 1)));
      DEBUG_PRINT("[INSTR] SBC A, $%02X\n", u8);
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0xA7:  // AND A, A
      RES_N;
      SET_H;
      RES_C;
      A &= A;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA0:  // AND A, B
      RES_N;
      SET_H;
      RES_C;
      A &= B;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA1:  // AND A, C
      RES_N;
      SET_H;
      RES_C;
      A &= C;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA2:  // AND A, D
      RES_N;
      SET_H;
      RES_C;
      A &= D;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA3:  // AND A, E
      RES_N;
      SET_H;
      RES_C;
      A &= E;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA4:  // AND A, H
      RES_N;
      SET_H;
      RES_C;
      A &= H;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA5:  // AND A, L
      RES_N;
      SET_H;
      RES_C;
      A &= L;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA6:  // AND A, (HL)
      RES_N;
      SET_H;
      RES_C;
      A &= ram[HL];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xE6:  // AND A, u8
      RES_N;
      SET_H;
      RES_C;
      A &= ram[++*pc];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] AND A, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;

    case 0xB7:  // OR A, A
      RES_N;
      RES_H;
      RES_C;
      A |= A;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB0:  // OR A, B
      RES_N;
      RES_H;
      RES_C;
      A |= B;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB1:  // OR A, C
      RES_N;
      RES_H;
      RES_C;
      A |= C;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB2:  // OR A, D
      RES_N;
      RES_H;
      RES_C;
      A |= D;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB3:  // OR A, E
      RES_N;
      RES_H;
      RES_C;
      A |= E;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB4:  // OR A, H
      RES_N;
      RES_H;
      RES_C;
      A |= H;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB5:  // OR A, L
      RES_N;
      RES_H;
      RES_C;
      A |= L;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB6:  // OR A, (HL)
      RES_N;
      RES_H;
      RES_C;
      A |= ram[HL];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xF6:  // OR A, u8
      RES_N;
      RES_H;
      RES_C;
      A |= ram[++*pc];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] OR A, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;

    case 0xAF:  // XOR A, A
      RES_N;
      RES_H;
      RES_C;
      A ^= A;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA8:  // XOR A, B
      RES_N;
      RES_H;
      RES_C;
      A ^= B;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xA9:  // XOR A, C
      RES_N;
      RES_H;
      RES_C;
      A ^= C;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xAA:  // XOR A, D
      RES_N;
      RES_H;
      RES_C;
      A ^= D;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xAB:  // XOR A, E
      RES_N;
      RES_H;
      RES_C;
      A ^= E;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xAC:  // XOR A, H
      RES_N;
      RES_H;
      RES_C;
      A ^= H;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xAD:  // XOR A, L
      RES_N;
      RES_H;
      RES_C;
      A ^= L;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xAE:  // XOR A, (HL)
      RES_N;
      RES_H;
      RES_C;
      A ^= ram[HL];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xEE:  // XOR A, u8
      RES_N;
      RES_H;
      RES_C;
      A ^= ram[++*pc];
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] XOR A, $%02X\n", ram[*pc]);
      ++*pc;
      *cycles = 8;
      break;

    case 0xBF:  // CP A, A
      SET_Z;    // A == A is always true
      SET_N;
      RES_C;  // A < A is always false
      IF_H(HALFCARRY_8(A, (~A + 1)));
      DEBUG_PRINT("[INSTR] CP A, A\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB8:  // CP A, B
      SET_N;
      IF_Z(A == B);
      IF_H(HALFCARRY_8(A, (~B + 1)));
      IF_C(A < B);
      DEBUG_PRINT("[INSTR] CP A, B\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xB9:  // CP A, C
      SET_N;
      IF_Z(A == C);
      IF_H(HALFCARRY_8(A, (~C + 1)));
      IF_C(A < C);
      DEBUG_PRINT("[INSTR] CP A, C\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xBA:  // CP A, D
      SET_N;
      IF_Z(A == D);
      IF_H(HALFCARRY_8(A, (~D + 1)));
      IF_C(A < D);
      DEBUG_PRINT("[INSTR] CP A, D\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xBB:  // CP A, E
      SET_N;
      IF_Z(A == E);
      IF_H(HALFCARRY_8(A, (~E + 1)));
      IF_C(A < E);
      DEBUG_PRINT("[INSTR] CP A, E\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xBC:  // CP A, H
      SET_N;
      IF_Z(A == H);
      IF_H(HALFCARRY_8(A, (~H + 1)));
      IF_C(A < H);
      DEBUG_PRINT("[INSTR] CP A, H\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xBD:  // CP A, L
      SET_N;
      IF_Z(A == L);
      IF_H(HALFCARRY_8(A, (~L + 1)));
      IF_C(A < L);
      DEBUG_PRINT("[INSTR] CP A, L\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0xBE:  // CP A, (HL)
      SET_N;
      IF_Z(A == ram[HL]);
      IF_H(HALFCARRY_8(A, (~ram[HL] + 1)));
      IF_C(A < ram[HL]);
      DEBUG_PRINT("[INSTR] CP A, (HL)\n");
      ++*pc;
      *cycles = 8;
      break;
    case 0xFE:  // CP A, u8
    {
      SET_N;
      uint8_t u8 = ram[++*pc];
      IF_Z(A == u8);
      IF_H(HALFCARRY_8(A, (~u8 + 1)));
      IF_C(A < u8);
      DEBUG_PRINT("[INSTR] CP A, $%02X\n", u8);
      ++*pc;
      *cycles = 4;
      break;
    }

    case 0x3C:  //  INC A
    {
      RES_N;
      A++;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, 1));
      DEBUG_PRINT("[INSTR] INC A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x04:  //  INC B
    {
      RES_N;
      B++;
      IF_Z(!B);
      IF_H(HALFCARRY_8(B, 1));
      DEBUG_PRINT("[INSTR] INC B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x0C:  //  INC C
    {
      RES_N;
      C++;
      IF_Z(!C);
      IF_H(HALFCARRY_8(C, 1));
      DEBUG_PRINT("[INSTR] INC C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x14:  //  INC D
    {
      RES_N;
      D++;
      IF_Z(!D);
      IF_H(HALFCARRY_8(D, 1));
      DEBUG_PRINT("[INSTR] INC D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x1C:  //  INC E
    {
      RES_N;
      E++;
      IF_Z(!E);
      IF_H(HALFCARRY_8(E, 1));
      DEBUG_PRINT("[INSTR] INC E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x24:  //  INC H
    {
      RES_N;
      H++;
      IF_Z(!H);
      IF_H(HALFCARRY_8(H, 1));
      DEBUG_PRINT("[INSTR] INC H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x2C:  //  INC L
    {
      RES_N;
      L++;
      IF_Z(!L);
      IF_H(HALFCARRY_8(L, 1));
      DEBUG_PRINT("[INSTR] INC L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x34:  //  INC (HL)
    {
      RES_N;
      ram[HL]++;
      IF_Z(!ram[HL]);
      IF_H(HALFCARRY_8(ram[HL], 1));
      DEBUG_PRINT("[INSTR] INC (HL)\n");
      ++*pc;
      *cycles = 12;
      break;
    }

    case 0x3D:  //  DEC A
    {
      SET_N;
      A--;
      IF_Z(!A);
      IF_H(HALFCARRY_8(A, 0xFF));
      DEBUG_PRINT("[INSTR] DEC A\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x05:  //  DEC B
    {
      SET_N;
      B--;
      IF_Z(!B);
      IF_H(HALFCARRY_8(B, 0xFF));
      DEBUG_PRINT("[INSTR] DEC B\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x0D:  //  DEC C
    {
      SET_N;
      C--;
      IF_Z(!C);
      IF_H(HALFCARRY_8(C, 0xFF));
      DEBUG_PRINT("[INSTR] DEC C\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x15:  //  DEC D
    {
      SET_N;
      D--;
      IF_Z(!D);
      IF_H(HALFCARRY_8(D, 0xFF));
      DEBUG_PRINT("[INSTR] DEC D\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x1D:  //  DEC E
    {
      SET_N;
      E--;
      IF_Z(!E);
      IF_H(HALFCARRY_8(E, 0xFF));
      DEBUG_PRINT("[INSTR] DEC E\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x25:  //  DEC H
    {
      SET_N;
      H--;
      IF_Z(!H);
      IF_H(HALFCARRY_8(H, 0xFF));
      DEBUG_PRINT("[INSTR] DEC H\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x2D:  //  DEC L
    {
      SET_N;
      L--;
      IF_Z(!L);
      IF_H(HALFCARRY_8(L, 0xFF));
      DEBUG_PRINT("[INSTR] DEC L\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x35:  //  DEC (HL)
    {
      SET_N;
      ram[HL]--;
      IF_Z(!ram[HL]);
      IF_H(HALFCARRY_8(ram[HL], 0xFF));
      DEBUG_PRINT("[INSTR] DEC (HL)\n");
      ++*pc;
      *cycles = 12;
      break;
    }

    /*-------------------
     *  16-Bit Arithmetic
     *-------------------*/
    case 0x09:  // ADD HL, BC
    {
      RES_N;
      uint16_t u16 = HL + BC;
      H = u16 >> 010;
      L = u16 & 0xFF;
      IF_H(HALFCARRY_16(HL, BC));
      IF_C(CARRY_16(HL, BC));
      DEBUG_PRINT("[INSTR] ADD HL, BC\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x19:  // ADD HL, DE
    {
      RES_N;
      uint16_t u16 = HL + DE;
      H = u16 >> 010;
      L = u16 & 0xFF;
      IF_H(HALFCARRY_16(HL, DE));
      IF_C(CARRY_16(HL, DE));
      DEBUG_PRINT("[INSTR] ADD HL, DE\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x29:  // ADD HL, HL
    {
      RES_N;
      uint16_t u16 = HL + HL;
      H = u16 >> 010;
      L = u16 & 0xFF;
      IF_H(HALFCARRY_16(HL, HL));
      IF_C(CARRY_16(HL, HL));
      DEBUG_PRINT("[INSTR] ADD HL, HL\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x39:  // ADD HL, SP
    {
      RES_N;
      uint16_t u16 = HL + *sp;
      IF_H(HALFCARRY_16(HL, *sp));
      IF_C(CARRY_16(HL, *sp));
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] ADD HL, SP\n");
      ++*pc;
      *cycles = 8;
      break;
    }

    case 0xE8:  // ADD SP, i8
      RES_Z;
      RES_N;
      int8_t i8 = ram[++*pc];
      IF_H(HALFCARRY_16(*sp, i8));
      IF_C(CARRY_16(*sp, i8));
      *sp += i8;
      DEBUG_PRINT("[INSTR] ADD SP, $%02X\n", i8);
      ++*pc;
      *cycles = 16;
      break;

    case 0x03:  // INC BC
    {
      uint16_t u16 = BC + 1;
      B = u16 >> 010;
      C = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] INC BC\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x13:  // INC DE
    {
      uint16_t u16 = DE + 1;
      D = u16 >> 010;
      E = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] INC DE\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x23:  // INC HL
    {
      uint16_t u16 = HL + 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] INC HL\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x33:  // INC SP
      ++*sp;
      DEBUG_PRINT("[INSTR] INC SP\n");
      ++*pc;
      *cycles = 8;
      break;

    case 0x0B:  // DEC BC
    {
      uint16_t u16 = BC - 1;
      B = u16 >> 010;
      C = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] DEC BC\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x1B:  // DEC DE
    {
      uint16_t u16 = DE - 1;
      D = u16 >> 010;
      E = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] DEC DE\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x2B:  // DEC HL
    {
      uint16_t u16 = HL - 1;
      H = u16 >> 010;
      L = u16 & 0xFF;
      DEBUG_PRINT("[INSTR] DEC HL\n");
      ++*pc;
      *cycles = 8;
      break;
    }
    case 0x3B:  // DEC SP
      --*sp;
      DEBUG_PRINT("[INSTR] DEC SP\n");
      ++*pc;
      *cycles = 8;
      break;

    /*------
     *  Misc
     *------*/
    case 0x27:  //  DAA
    {
      RES_H;
      uint8_t correction = 0;

      if (!GET_N && (A & 0xF) > 0x9) correction |= 0x6;

      if (!GET_C || (!GET_N && A > 0x99)) {
        correction |= 0x60;
        SET_C;
      }

      A += GET_N ? -correction : correction;
      IF_Z(!A);

      DEBUG_PRINT("[INSTR] DAA\n");
      ++*pc;
      *cycles = 27;
      break;
    }

    case 0x2F:  // CPL
      SET_H;
      SET_N;
      A = ~A;
      DEBUG_PRINT("[INSTR] CPL\n");
      ++*pc;
      *cycles = 4;
      break;

    case 0x3F:  // CCF
      RES_N;
      RES_H;
      IF_C(!GET_C);
      DEBUG_PRINT("[INSTR] CCF\n");
      ++*pc;
      *cycles = 4;
      break;

    case 0x37:  // SCF
      RES_N;
      RES_H;
      SET_C;
      DEBUG_PRINT("[INSTR] SCF\n");
      ++*pc;
      *cycles = 4;
      break;

    case 0x76:  // HALT
      *hlt = true;
      printf("[INFO] HALT\n");
      ++*pc;
      *cycles = 4;
      break;

    case 0x10:  // STOP
      *hlt = true;
      printf("[INFO] STOP\n");
      CoreDump("core-GameboyEmulator.dmp", ram);
      *pc += 2;  //??
      *cycles = 4;
      break;

    case 0xF3:  // DI
      *IME = false;
      DEBUG_PRINT("[INSTR] DI\n");
      ++*pc;
      *cycles = 4;
      break;

    case 0xFB:  // EI
      *IME = true;
      DEBUG_PRINT("[INSTR] EI\n");
      ++*pc;
      *cycles = 4;
      break;

    /*------------------
     *  Rotates & Shifts
     *------------------*/
    case 0x07:  // RLCA
      RES_N;
      RES_H;
      IF_C((A & 0x80) != 0);
      A = (A << 1) | GET_C;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] RLCA\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x17:  // RLA
    {
      RES_N;
      RES_H;
      uint8_t carry = GET_C;
      IF_C((A & 0x80) != 0);
      A = A << 1 | carry;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] RLA\n");
      ++*pc;
      *cycles = 4;
      break;
    }
    case 0x0F:  // RRCA
      RES_N;
      RES_H;
      IF_C((A & 0x01) != 0);
      A = (A >> 1) | GET_C << 7;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] RRCA\n");
      ++*pc;
      *cycles = 4;
      break;
    case 0x1F:  // RRA
    {
      RES_N;
      RES_H;
      uint8_t carry = GET_C << 7;
      IF_C((A & 0x01) != 0);
      A = A >> 1 | carry;
      IF_Z(!A);
      DEBUG_PRINT("[INSTR] RRA\n");
      ++*pc;
      *cycles = 4;
      break;
    }

    /*-------
     *  Jumps
     *-------*/
    case 0xC3:  // JP u16
      *pc = ram[*pc + 1] | ram[*pc + 2] << 010;
      DEBUG_PRINT("[INSTR] JP $%04X\n", *pc);
      *cycles = 16;
      break;

    case 0xC2:  // JP NZ, u16
      if (!GET_Z)
        *pc = ram[*pc + 1] | ram[*pc + 2] << 010;
      else
        *pc += 3;
      DEBUG_PRINT("[INSTR] JP NZ, $%04X\n", ram[*pc] | ram[*pc + 1] << 010);
      *cycles = 12;
      break;
    case 0xCA:  // JP Z, u16
      if (GET_Z)
        *pc = ram[*pc + 1] | ram[*pc + 2] << 010;
      else
        *pc += 3;
      DEBUG_PRINT("[INSTR] JP Z, $%04X\n", ram[*pc] | ram[*pc + 1] << 010);
      *cycles = 12;
      break;
    case 0xD2:  // JP NC, u16
      if (!GET_C)
        *pc = ram[*pc + 1] | ram[*pc + 2] << 010;
      else
        *pc += 3;
      DEBUG_PRINT("[INSTR] JP NC, $%04X\n", ram[*pc] | ram[*pc + 1] << 010);
      *cycles = 12;
      break;
    case 0xDA:  // JP C, u16
      if (GET_C)
        *pc = ram[*pc + 1] | ram[*pc + 2] << 010;
      else
        *pc += 3;
      DEBUG_PRINT("[INSTR] JP C, $%04X\n", ram[*pc] | ram[*pc + 1] << 010);
      *cycles = 12;
      break;

    case 0xE9:  // JP (HL)
      *pc = HL;
      DEBUG_PRINT("[INSTR] JP (HL)\n");
      *cycles = 4;
      break;

    case 0x18:  // JR i8
    {
      int8_t i8 = ram[++*pc];
      *pc += i8;
      ++*pc;
      DEBUG_PRINT("[INSTR] JR $%04X\n", *pc);
      *cycles = 8;
      break;
    }

    case 0x20:  // JR NZ, i8
    {
      int8_t i8 = ram[++*pc];
      uint16_t addr = *pc + i8;
      if (!GET_Z) *pc = addr;
      ++*pc;
      DEBUG_PRINT("[INSTR] JR NZ, $%04X\n", addr);
      *cycles = 8;
      break;
    }
    case 0x28:  // JR Z, i8
    {
      int8_t i8 = ram[++*pc];
      uint16_t addr = *pc + i8;
      if (GET_Z) *pc = addr;
      ++*pc;
      DEBUG_PRINT("[INSTR] JR Z, $%04X\n", addr);
      *cycles = 8;
      break;
    }
    case 0x30:  // JR NC, i8
    {
      int8_t i8 = ram[++*pc];
      uint16_t addr = *pc + i8;
      if (!GET_C) *pc = addr;
      ++*pc;
      DEBUG_PRINT("[INSTR] JR NC, $%04X\n", addr);
      *cycles = 8;
      break;
    }
    case 0x38:  // JR C, i8
    {
      int8_t i8 = ram[++*pc];
      uint16_t addr = *pc + i8;
      if (GET_C) *pc = addr;
      ++*pc;
      DEBUG_PRINT("[INSTR] JR C, $%04X\n", addr);
      *cycles = 8;
      break;
    }

      /*-------
       *  Calls
       *-------*/
    case 0xCD:  // CALL u16
    {
      uint16_t address = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 3;
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = address;
      DEBUG_PRINT("[INSTR] CALL $%04X\n", address);
      *cycles = 12;
      break;
    }

    case 0xC4:  // CALL NZ, u16
    {
      // op
      uint16_t address = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 3;
      if (!GET_Z) {
        ram[--*sp] = *pc >> 010;
        ram[--*sp] = *pc & 0xFF;
        *pc = address;
      }
      DEBUG_PRINT("[INSTR] CALL NZ, $%04X\n", address);
      *cycles = 12;
      break;
    }
    case 0xCC:  // CALL Z, u16
    {
      // op
      uint16_t address = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 3;
      if (GET_Z) {
        ram[--*sp] = *pc >> 010;
        ram[--*sp] = *pc & 0xFF;
        *pc = address;
      }
      DEBUG_PRINT("[INSTR] CALL Z, $%04X\n", address);
      *cycles = 12;
      break;
    }
    case 0xD4:  // CALL NC, u16
    {
      // op
      uint16_t address = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 3;
      if (!GET_C) {
        ram[--*sp] = *pc >> 010;
        ram[--*sp] = *pc & 0xFF;
        *pc = address;
      }
      DEBUG_PRINT("[INSTR] CALL NC, $%04X\n", address);
      *cycles = 12;
      break;
    }
    case 0xDC:  // CALL C, u16
    {
      // op
      uint16_t address = ram[*pc + 1] | ram[*pc + 2] << 010;
      *pc += 3;
      if (GET_C) {
        ram[--*sp] = *pc >> 010;
        ram[--*sp] = *pc & 0xFF;
        *pc = address;
      }
      DEBUG_PRINT("[INSTR] CALL C, $%04X\n", address);
      *cycles = 12;
      break;
    }

      /*----------
       *  Restarts
       *----------*/
    case 0xC7:  // RST 00h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x00;
      DEBUG_PRINT("[INSTR] RST 00h\n");
      *cycles = 32;
      break;
    case 0xCF:  // RST 08h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x08;
      DEBUG_PRINT("[INSTR] RST 08h\n");
      *cycles = 32;
      break;
    case 0xD7:  // RST 10h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x10;
      DEBUG_PRINT("[INSTR] RST 10h\n");
      *cycles = 32;
      break;
    case 0xDF:  // RST 18h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x18;
      DEBUG_PRINT("[INSTR] RST 18h\n");
      *cycles = 32;
      break;
    case 0xE7:  // RST 20h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x20;
      DEBUG_PRINT("[INSTR] RST 20h\n");
      *cycles = 32;
      break;
    case 0xEF:  // RST 28h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x28;
      DEBUG_PRINT("[INSTR] RST 28h\n");
      *cycles = 32;
      break;
    case 0xF7:  // RST 30h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x30;
      DEBUG_PRINT("[INSTR] RST 30h\n");
      *cycles = 32;
      break;
    case 0xFF:  // RST 38h
      ram[--*sp] = *pc >> 010;
      ram[--*sp] = *pc & 0xFF;
      *pc = 0x38;
      DEBUG_PRINT("[INSTR] RST 38h\n");
      *cycles = 32;
      break;

      /*---------
       *  Returns
       *--------*/
    case 0xC9:  // RET
      *pc = ram[*sp] | ram[*sp + 1] << 010;
      *sp += 2;
      DEBUG_PRINT("[INSTR] RET\n");
      *cycles = 8;
      break;

    case 0xC0:  // RET NZ
      if (!GET_Z) {
        *pc = ram[*sp] | ram[*sp + 1] << 010;
        *sp += 2;
      } else
        ++*pc;
      DEBUG_PRINT("[INSTR] RET NZ\n");
      *cycles = 8;
      break;
    case 0xC8:  // RET Z
      if (GET_Z) {
        *pc = ram[*sp] | ram[*sp + 1] << 010;
        *sp += 2;
      } else
        ++*pc;
      DEBUG_PRINT("[INSTR] RET Z\n");
      *cycles = 8;
      break;
    case 0xD0:  // RET NC
      if (!GET_C) {
        *pc = ram[*sp] | ram[*sp + 1] << 010;
        *sp += 2;
      } else
        ++*pc;
      DEBUG_PRINT("[INSTR] RET NC\n");
      *cycles = 8;
      break;
    case 0xD8:  // RET C
      if (GET_C) {
        *pc = ram[*sp] | ram[*sp + 1] << 010;
        *sp += 2;
      } else
        ++*pc;
      DEBUG_PRINT("[INSTR] RET C\n");
      *cycles = 8;
      break;

    case 0xD9:  // RETI
      *pc = ram[*sp] | ram[*sp + 1] << 010;
      *sp += 2;
      *IME = true;
      DEBUG_PRINT("[INSTR] RETI\n");
      *cycles = 8;
      break;

    /*---- PREF 0xCB -------------------
     *  Prefix for extended instructions
     *----------------------------------*/
    case 0XCB:
      opcode = ram[++*pc];
      switch (opcode) {
          /*------
           *  Misc
           *------*/
        case 0x37:  // SWAP A
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!A) {
            SET_Z;
          } else {
            A = (A >> 4) | (A << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x30:  // SWAP B
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!B) {
            SET_Z;
          } else {
            B = (B >> 4) | (B << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x31:  // SWAP C
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!C) {
            SET_Z;
          } else {
            C = (C >> 4) | (C << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x32:  // SWAP D
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!D) {
            SET_Z;
          } else {
            D = (D >> 4) | (D << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x33:  // SWAP E
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!E) {
            SET_Z;
          } else {
            E = (E >> 4) | (E << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x34:  // SWAP H
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!H) {
            SET_Z;
          } else {
            H = (H >> 4) | (H << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x35:  // SWAP L
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!L) {
            SET_Z;
          } else {
            L = (L >> 4) | (L << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x36:  // SWAP (HL)
          RES_N;
          RES_H;
          RES_C;
          RES_Z;
          if (!ram[HL]) {
            SET_Z;
          } else {
            ram[HL] = (ram[HL] >> 4) | (ram[HL] << 4);
          }
          DEBUG_PRINT("[INSTR] SWAP L\n");
          ++*pc;
          *cycles = 12;
          break;

          /*-----------------
           * Rotates & Shifts
           *-----------------*/

        case 0x07:  // RLC A
          RES_N;
          RES_H;
          IF_C((A & 0x80) != 0);
          A = (A << 1) | GET_C;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] RLC A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x00:  // RLC B
          RES_N;
          RES_H;
          IF_C((B & 0x80) != 0);
          B = (B << 1) | GET_C;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] RLC B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x01:  // RLC C
          RES_N;
          RES_H;
          IF_C((C & 0x80) != 0);
          C = (C << 1) | GET_C;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] RLC C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x02:  // RLC D
          RES_N;
          RES_H;
          IF_C((D & 0x80) != 0);
          D = (D << 1) | GET_C;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] RLC D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x03:  // RLC E
          RES_N;
          RES_H;
          IF_C((E & 0x80) != 0);
          E = (E << 1) | GET_C;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] RLC E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x04:  // RLC H
          RES_N;
          RES_H;
          IF_C((H & 0x80) != 0);
          H = (H << 1) | GET_C;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] RLC H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x05:  // RLC L
          RES_N;
          RES_H;
          IF_C((L & 0x80) != 0);
          L = (L << 1) | GET_C;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] RLC L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x06:  // RLC (HL)
          RES_N;
          RES_H;
          IF_C((ram[HL] & 0x80) != 0);
          ram[HL] = (ram[HL] << 1) | GET_C;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] RLC (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

        case 0x17:  // RL A
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((A & 0x80) != 0);
          A = A << 1 | carry;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] RL A\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x10:  // RL B
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((B & 0x80) != 0);
          B = B << 1 | carry;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] RL B\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x11:  // RL C
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((C & 0x80) != 0);
          C = C << 1 | carry;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] RL C\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x12:  // RL D
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((D & 0x80) != 0);
          D = D << 1 | carry;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] RL D\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x13:  // RL E
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((E & 0x80) != 0);
          E = E << 1 | carry;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] RL E\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x14:  // RL H
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((H & 0x80) != 0);
          H = H << 1 | carry;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] RL H\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x15:  // RL L
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((L & 0x80) != 0);
          L = L << 1 | carry;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] RL L\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x16:  // RL (HL)
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C;
          IF_C((ram[HL] & 0x80) != 0);
          ram[HL] = ram[HL] << 1 | carry;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] RL (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        }

        case 0x0F:  // RRC A
          RES_N;
          RES_H;
          IF_C((A & 0x01) != 0);
          A = (A >> 1) | GET_C << 7;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] RRC A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x08:  // RRC B
          RES_N;
          RES_H;
          IF_C((B & 0x01) != 0);
          B = (B >> 1) | GET_C << 7;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] RRC B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x09:  // RRC C
          RES_N;
          RES_H;
          IF_C((C & 0x01) != 0);
          C = (C >> 1) | GET_C << 7;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] RRC C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x0A:  // RRC D
          RES_N;
          RES_H;
          IF_C((D & 0x01) != 0);
          D = (D >> 1) | GET_C << 7;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] RRC D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x0B:  // RRC E
          RES_N;
          RES_H;
          IF_C((E & 0x01) != 0);
          E = (E >> 1) | GET_C << 7;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] RRC E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x0C:  // RRC H
          RES_N;
          RES_H;
          IF_C((H & 0x01) != 0);
          H = (H >> 1) | GET_C << 7;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] RRC H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x0D:  // RRC L
          RES_N;
          RES_H;
          IF_C((L & 0x01) != 0);
          L = (L >> 1) | GET_C << 7;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] RRC L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x0E:  // RRC (HL)
          RES_N;
          RES_H;
          IF_C((ram[HL] & 0x01) != 0);
          ram[HL] = (ram[HL] >> 1) | GET_C << 7;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] RRC (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

        case 0x1F:  // RR A
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((A & 0x01) != 0);
          A = A >> 1 | carry;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] RR A\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x18:  // RR B
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((B & 0x01) != 0);
          B = B >> 1 | carry;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] RR B\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x19:  // RR C
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((C & 0x01) != 0);
          C = C >> 1 | carry;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] RR C\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x1A:  // RR D
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((D & 0x01) != 0);
          D = D >> 1 | carry;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] RR D\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x1B:  // RR E
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((E & 0x01) != 0);
          E = E >> 1 | carry;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] RR E\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x1C:  // RR H
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((H & 0x01) != 0);
          H = H >> 1 | carry;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] RR H\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x1D:  // RR L
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((L & 0x01) != 0);
          L = L >> 1 | carry;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] RR L\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x1E:  // RR (HL)
        {
          RES_N;
          RES_H;
          uint8_t carry = GET_C << 7;
          IF_C((ram[HL] & 0x01) != 0);
          ram[HL] = ram[HL] >> 1 | carry;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] RR (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        }

        case 0x27:  //  SLA A
        {
          RES_N;
          RES_H;
          IF_C((A & 0x80) == 0x80);
          A = A << 1;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] SLA A\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x20:  //  SLA B
        {
          RES_N;
          RES_H;
          IF_C((B & 0x80) == 0x80);
          B = B << 1;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] SLA B\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x21:  //  SLA C
        {
          RES_N;
          RES_H;
          IF_C((C & 0x80) == 0x80);
          C = C << 1;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] SLA C\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x22:  //  SLA D
        {
          RES_N;
          RES_H;
          IF_C((D & 0x80) == 0x80);
          D = D << 1;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] SLA D\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x23:  //  SLA E
        {
          RES_N;
          RES_H;
          IF_C((E & 0x80) == 0x80);
          E = E << 1;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] SLA E\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x24:  //  SLA H
        {
          RES_N;
          RES_H;
          IF_C((H & 0x80) == 0x80);
          H = H << 1;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] SLA H\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x25:  //  SLA L
        {
          RES_N;
          RES_H;
          IF_C((L & 0x80) == 0x80);
          L = L << 1;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] SLA L\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x26:  //  SLA (HL)
        {
          RES_N;
          RES_H;
          IF_C((ram[HL] & 0x80) == 0x80);
          ram[HL] = ram[HL] << 1;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] SLA (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        }

        case 0x2F:  //  SRA A
        {
          RES_N;
          RES_H;
          IF_C((A & 1) == 1);
          uint8_t msb = A & 0x80;
          A = A >> 1 | msb;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] SRA A\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x28:  //  SRA B
        {
          RES_N;
          RES_H;
          IF_C((B & 1) == 1);
          uint8_t msb = B & 0x80;
          B = B >> 1 | msb;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] SRA B\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x29:  //  SRA C
        {
          RES_N;
          RES_H;
          IF_C((C & 1) == 1);
          uint8_t msb = C & 0x80;
          C = C >> 1 | msb;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] SRA C\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x2A:  //  SRA D
        {
          RES_N;
          RES_H;
          IF_C((D & 1) == 1);
          uint8_t msb = D & 0x80;
          D = D >> 1 | msb;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] SRA D\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x2B:  //  SRA E
        {
          RES_N;
          RES_H;
          IF_C((E & 1) == 1);
          uint8_t msb = E & 0x80;
          E = E >> 1 | msb;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] SRA E\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x2C:  //  SRA H
        {
          RES_N;
          RES_H;
          IF_C((H & 1) == 1);
          uint8_t msb = H & 0x80;
          H = H >> 1 | msb;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] SRA H\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x2D:  //  SRA L
        {
          RES_N;
          RES_H;
          IF_C((L & 1) == 1);
          uint8_t msb = L & 0x80;
          L = L >> 1 | msb;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] SRA L\n");
          ++*pc;
          *cycles = 8;
          break;
        }
        case 0x2E:  //  SRA (HL)
        {
          RES_N;
          RES_H;
          IF_C((ram[HL] & 1) == 1);
          uint8_t msb = ram[HL] & 0x80;
          ram[HL] = ram[HL] >> 1 | msb;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] SRA (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        }

        case 0x3F:  //  SRL A
          RES_N;
          RES_H;
          IF_C((A & 1) == 1);
          A = A >> 1;
          IF_Z(!A);
          DEBUG_PRINT("[INSTR] SRL A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x38:  //  SRL B
          RES_N;
          RES_H;
          IF_C((B & 1) == 1);
          B = B >> 1;
          IF_Z(!B);
          DEBUG_PRINT("[INSTR] SRL B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x39:  //  SRL C
          RES_N;
          RES_H;
          IF_C((C & 1) == 1);
          C = C >> 1;
          IF_Z(!C);
          DEBUG_PRINT("[INSTR] SRL C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x3A:  //  SRL D
          RES_N;
          RES_H;
          IF_C((D & 1) == 1);
          D = D >> 1;
          IF_Z(!D);
          DEBUG_PRINT("[INSTR] SRL D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x3B:  //  SRL E
          RES_N;
          RES_H;
          IF_C((E & 1) == 1);
          E = E >> 1;
          IF_Z(!E);
          DEBUG_PRINT("[INSTR] SRL E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x3C:  //  SRL H
          RES_N;
          RES_H;
          IF_C((H & 1) == 1);
          H = H >> 1;
          IF_Z(!H);
          DEBUG_PRINT("[INSTR] SRL H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x3D:  //  SRL L
          RES_N;
          RES_H;
          IF_C((L & 1) == 1);
          L = L >> 1;
          IF_Z(!L);
          DEBUG_PRINT("[INSTR] SRL L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x3E:  //  SRL (HL)
          RES_N;
          RES_H;
          IF_C((ram[HL] & 1) == 1);
          ram[HL] = ram[HL] >> 1;
          IF_Z(!ram[HL]);
          DEBUG_PRINT("[INSTR] SRL (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

          /*-------------
           *  Bit Opcodes
           *-------------*/
        case 0x47:  // BIT 0, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, A));
          DEBUG_PRINT("[INSTR] BIT 0, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x40:  // BIT 0, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, B));
          DEBUG_PRINT("[INSTR] BIT 0, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x41:  // BIT 0, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, C));
          DEBUG_PRINT("[INSTR] BIT 0, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x42:  // BIT 0, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, D));
          DEBUG_PRINT("[INSTR] BIT 0, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x43:  // BIT 0, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, E));
          DEBUG_PRINT("[INSTR] BIT 0, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x44:  // BIT 0, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, H));
          DEBUG_PRINT("[INSTR] BIT 0, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x45:  // BIT 0, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, L));
          DEBUG_PRINT("[INSTR] BIT 0, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x46:  // BIT 0, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(0, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 0, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x4F:  // BIT 1, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, A));
          DEBUG_PRINT("[INSTR] BIT 1, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x48:  // BIT 1, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, B));
          DEBUG_PRINT("[INSTR] BIT 1, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x49:  // BIT 1, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, C));
          DEBUG_PRINT("[INSTR] BIT 1, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x4A:  // BIT 1, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, D));
          DEBUG_PRINT("[INSTR] BIT 1, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x4B:  // BIT 1, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, E));
          DEBUG_PRINT("[INSTR] BIT 1, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x4C:  // BIT 1, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, H));
          DEBUG_PRINT("[INSTR] BIT 1, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x4D:  // BIT 1, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, L));
          DEBUG_PRINT("[INSTR] BIT 1, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x4E:  // BIT 1, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(1, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 1, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x57:  // BIT 2, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, A));
          DEBUG_PRINT("[INSTR] BIT 2, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x50:  // BIT 2, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, B));
          DEBUG_PRINT("[INSTR] BIT 2, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x51:  // BIT 2, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, C));
          DEBUG_PRINT("[INSTR] BIT 2, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x52:  // BIT 2, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, D));
          DEBUG_PRINT("[INSTR] BIT 2, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x53:  // BIT 2, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, E));
          DEBUG_PRINT("[INSTR] BIT 2, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x54:  // BIT 2, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, H));
          DEBUG_PRINT("[INSTR] BIT 2, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x55:  // BIT 2, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, L));
          DEBUG_PRINT("[INSTR] BIT 2, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x56:  // BIT 2, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(2, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 2, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x5F:  // BIT 3, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, A));
          DEBUG_PRINT("[INSTR] BIT 3, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x58:  // BIT 3, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, B));
          DEBUG_PRINT("[INSTR] BIT 3, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x59:  // BIT 3, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, C));
          DEBUG_PRINT("[INSTR] BIT 3, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x5A:  // BIT 3, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, D));
          DEBUG_PRINT("[INSTR] BIT 3, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x5B:  // BIT 3, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, E));
          DEBUG_PRINT("[INSTR] BIT 3, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x5C:  // BIT30, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, H));
          DEBUG_PRINT("[INSTR] BIT 3, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x5D:  // BIT 3, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, L));
          DEBUG_PRINT("[INSTR] BIT 3, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x5E:  // BIT 3, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(3, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 3, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x67:  // BIT 4, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, A));
          DEBUG_PRINT("[INSTR] BIT 4, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x60:  // BIT 4, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, B));
          DEBUG_PRINT("[INSTR] BIT 4, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x61:  // BIT 4, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, C));
          DEBUG_PRINT("[INSTR] BIT 4, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x62:  // BIT 4, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, D));
          DEBUG_PRINT("[INSTR] BIT 4, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x63:  // BIT 4, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, E));
          DEBUG_PRINT("[INSTR] BIT 4, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x64:  // BIT 4, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, H));
          DEBUG_PRINT("[INSTR] BIT 4, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x65:  // BIT 4, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, L));
          DEBUG_PRINT("[INSTR] BIT 4, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x66:  // BIT 4, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(4, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 4, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x6F:  // BIT 5, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, A));
          DEBUG_PRINT("[INSTR] BIT 5, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x68:  // BIT 5, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, B));
          DEBUG_PRINT("[INSTR] BIT 5, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x69:  // BIT 5, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, C));
          DEBUG_PRINT("[INSTR] BIT 5, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x6A:  // BIT 5, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, D));
          DEBUG_PRINT("[INSTR] BIT 5, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x6B:  // BIT 5, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, E));
          DEBUG_PRINT("[INSTR] BIT 5, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x6C:  // BIT 5, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, H));
          DEBUG_PRINT("[INSTR] BIT 5, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x6D:  // BIT 5, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, L));
          DEBUG_PRINT("[INSTR] BIT 5, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x6E:  // BIT 5, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(5, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 5, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x77:  // BIT 6, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, A));
          DEBUG_PRINT("[INSTR] BIT 6, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x70:  // BIT 6, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, B));
          DEBUG_PRINT("[INSTR] BIT 6, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x71:  // BIT 6, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, C));
          DEBUG_PRINT("[INSTR] BIT 6, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x72:  // BIT 6, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, D));
          DEBUG_PRINT("[INSTR] BIT 6, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x73:  // BIT 6, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, E));
          DEBUG_PRINT("[INSTR] BIT 6, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x74:  // BIT 6, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, H));
          DEBUG_PRINT("[INSTR] BIT 6, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x75:  // BIT 6, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, L));
          DEBUG_PRINT("[INSTR] BIT 6, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x76:  // BIT 6, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(6, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 6, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x7F:  // BIT 7, A
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, A));
          DEBUG_PRINT("[INSTR] BIT 7, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x78:  // BIT 7, B
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, B));
          DEBUG_PRINT("[INSTR] BIT 7, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x79:  // BIT 7, C
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, C));
          DEBUG_PRINT("[INSTR] BIT 7, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x7A:  // BIT 7, D
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, D));
          DEBUG_PRINT("[INSTR] BIT 7, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x7B:  // BIT 7, E
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, E));
          DEBUG_PRINT("[INSTR] BIT 7, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x7C:  // BIT 7, H
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, H));
          DEBUG_PRINT("[INSTR] BIT 7, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x7D:  // BIT 7, L
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, L));
          DEBUG_PRINT("[INSTR] BIT 7, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x7E:  // BIT 7, (HL)
          RES_N;
          SET_H;
          IF_Z(!CHECK_BIT(7, ram[HL]));
          DEBUG_PRINT("[INSTR] BIT 7, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

        case 0xC7:  // SET 0, A
          SET_BIT(0, A);
          DEBUG_PRINT("[INSTR] SET 0, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC0:  // SET 0, B
          SET_BIT(0, B);
          DEBUG_PRINT("[INSTR] SET 0, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC1:  // SET 0, C
          SET_BIT(0, C);
          DEBUG_PRINT("[INSTR] SET 0, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC2:  // SET 0, D
          SET_BIT(0, D);
          DEBUG_PRINT("[INSTR] SET 0, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC3:  // SET 0, E
          SET_BIT(0, E);
          DEBUG_PRINT("[INSTR] SET 0, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC4:  // SET 0, H
          SET_BIT(0, H);
          DEBUG_PRINT("[INSTR] SET 0, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC5:  // SET 0, L
          SET_BIT(0, L);
          DEBUG_PRINT("[INSTR] SET 0, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC6:  // SET 0, (HL)
          SET_BIT(0, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 0, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xCF:  // SET 1, A
          SET_BIT(1, A);
          DEBUG_PRINT("[INSTR] SET 1, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC8:  // SET 1, B
          SET_BIT(1, B);
          DEBUG_PRINT("[INSTR] SET 1, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xC9:  // SET 1, C
          SET_BIT(1, C);
          DEBUG_PRINT("[INSTR] SET 1, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xCA:  // SET 1, D
          SET_BIT(1, D);
          DEBUG_PRINT("[INSTR] SET 1, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xCB:  // SET 1, E
          SET_BIT(1, E);
          DEBUG_PRINT("[INSTR] SET 1, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xCC:  // SET 1, H
          SET_BIT(1, H);
          DEBUG_PRINT("[INSTR] SET 1, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xCD:  // SET 1, L
          SET_BIT(1, L);
          DEBUG_PRINT("[INSTR] SET 1, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xCE:  // SET 1, (HL)
          SET_BIT(1, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 1, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xD7:  // SET 2, A
          SET_BIT(2, A);
          DEBUG_PRINT("[INSTR] SET 2, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD0:  // SET 2, B
          SET_BIT(2, B);
          DEBUG_PRINT("[INSTR] SET 2, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD1:  // SET 2, C
          SET_BIT(2, C);
          DEBUG_PRINT("[INSTR] SET 2, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD2:  // SET 2, D
          SET_BIT(2, D);
          DEBUG_PRINT("[INSTR] SET 2, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD3:  // SET 2, E
          SET_BIT(2, E);
          DEBUG_PRINT("[INSTR] SET 2, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD4:  // SET 2, H
          SET_BIT(2, H);
          DEBUG_PRINT("[INSTR] SET 2, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD5:  // SET 2, L
          SET_BIT(2, L);
          DEBUG_PRINT("[INSTR] SET 2, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD6:  // SET 2, (HL)
          SET_BIT(2, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 2, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xDF:  // SET 3, A
          SET_BIT(3, A);
          DEBUG_PRINT("[INSTR] SET 3, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD8:  // SET 3, B
          SET_BIT(3, B);
          DEBUG_PRINT("[INSTR] SET 3, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xD9:  // SET 3, C
          SET_BIT(3, C);
          DEBUG_PRINT("[INSTR] SET 3, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xDA:  // SET 3, D
          SET_BIT(3, D);
          DEBUG_PRINT("[INSTR] SET 3, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xDB:  // SET 3, E
          SET_BIT(3, E);
          DEBUG_PRINT("[INSTR] SET 3, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xDC:  // SET 3, H
          SET_BIT(3, H);
          DEBUG_PRINT("[INSTR] SET 3, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xDD:  // SET 3, L
          SET_BIT(3, L);
          DEBUG_PRINT("[INSTR] SET 3, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xDE:  // SET 3, (HL)
          SET_BIT(3, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 3, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xE7:  // SET 4, A
          SET_BIT(4, A);
          DEBUG_PRINT("[INSTR] SET 4, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE0:  // SET 4, B
          SET_BIT(4, B);
          DEBUG_PRINT("[INSTR] SET 4, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE1:  // SET 4, C
          SET_BIT(4, C);
          DEBUG_PRINT("[INSTR] SET 4, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE2:  // SET 4, D
          SET_BIT(4, D);
          DEBUG_PRINT("[INSTR] SET 4, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE3:  // SET 4, E
          SET_BIT(4, E);
          DEBUG_PRINT("[INSTR] SET 4, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE4:  // SET 4, H
          SET_BIT(4, H);
          DEBUG_PRINT("[INSTR] SET 4, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE5:  // SET 4, L
          SET_BIT(4, L);
          DEBUG_PRINT("[INSTR] SET 4, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE6:  // SET 4, (HL)
          SET_BIT(4, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 4, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xEF:  // SET 5, A
          SET_BIT(5, A);
          DEBUG_PRINT("[INSTR] SET 5, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE8:  // SET 5, B
          SET_BIT(5, B);
          DEBUG_PRINT("[INSTR] SET 5, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xE9:  // SET 5, C
          SET_BIT(5, C);
          DEBUG_PRINT("[INSTR] SET 5, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xEA:  // SET 5, D
          SET_BIT(5, D);
          DEBUG_PRINT("[INSTR] SET 5, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xEB:  // SET 5, E
          SET_BIT(5, E);
          DEBUG_PRINT("[INSTR] SET 5, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xEC:  // SET 5, H
          SET_BIT(5, H);
          DEBUG_PRINT("[INSTR] SET 5, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xED:  // SET 5, L
          SET_BIT(5, L);
          DEBUG_PRINT("[INSTR] SET 5, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xEE:  // SET 5, (HL)
          SET_BIT(5, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 5, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xF7:  // SET 6, A
          SET_BIT(6, A);
          DEBUG_PRINT("[INSTR] SET 6, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF0:  // SET 6, B
          SET_BIT(6, B);
          DEBUG_PRINT("[INSTR] SET 6, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF1:  // SET 6, C
          SET_BIT(6, C);
          DEBUG_PRINT("[INSTR] SET 6, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF2:  // SET 6, D
          SET_BIT(6, D);
          DEBUG_PRINT("[INSTR] SET 6, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF3:  // SET 6, E
          SET_BIT(6, E);
          DEBUG_PRINT("[INSTR] SET 6, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF4:  // SET 6, H
          SET_BIT(6, H);
          DEBUG_PRINT("[INSTR] SET 6, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF5:  // SET 6, L
          SET_BIT(6, L);
          DEBUG_PRINT("[INSTR] SET 6, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF6:  // SET 6, (HL)
          SET_BIT(6, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 6, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xFF:  // SET 7, A
          SET_BIT(7, A);
          DEBUG_PRINT("[INSTR] SET 7, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF8:  // SET 7, B
          SET_BIT(7, B);
          DEBUG_PRINT("[INSTR] SET 7, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xF9:  // SET 7, C
          SET_BIT(7, C);
          DEBUG_PRINT("[INSTR] SET 7, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xFA:  // SET 7, D
          SET_BIT(7, D);
          DEBUG_PRINT("[INSTR] SET 7, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xFB:  // SET 7, E
          SET_BIT(7, E);
          DEBUG_PRINT("[INSTR] SET 7, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xFC:  // SET 7, H
          SET_BIT(7, H);
          DEBUG_PRINT("[INSTR] SET 7, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xFD:  // SET 7, L
          SET_BIT(7, L);
          DEBUG_PRINT("[INSTR] SET 7, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xFE:  // SET 7, (HL)
          SET_BIT(7, ram[HL]);
          DEBUG_PRINT("[INSTR] SET 7, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

        case 0x87:  // RES 0, A
          RES_BIT(0, A);
          DEBUG_PRINT("[INSTR] RES 0, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x80:  // RES 0, B
          RES_BIT(0, B);
          DEBUG_PRINT("[INSTR] RES 0, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x81:  // RES 0, C
          RES_BIT(0, C);
          DEBUG_PRINT("[INSTR] RES 0, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x82:  // RES 0, D
          RES_BIT(0, D);
          DEBUG_PRINT("[INSTR] RES 0, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x83:  // RES 0, E
          RES_BIT(0, E);
          DEBUG_PRINT("[INSTR] RES 0, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x84:  // RES 0, H
          RES_BIT(0, H);
          DEBUG_PRINT("[INSTR] RES 0, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x85:  // RES 0, L
          RES_BIT(0, L);
          DEBUG_PRINT("[INSTR] RES 0, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x86:  // RES 0, (HL)
          RES_BIT(0, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 0, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x8F:  // RES 1, A
          RES_BIT(1, A);
          DEBUG_PRINT("[INSTR] RES 1, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x88:  // RES 1, B
          RES_BIT(1, B);
          DEBUG_PRINT("[INSTR] RES 1, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x89:  // RES 1, C
          RES_BIT(1, C);
          DEBUG_PRINT("[INSTR] RES 1, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x8A:  // RES 1, D
          RES_BIT(1, D);
          DEBUG_PRINT("[INSTR] RES 1, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x8B:  // RES 1, E
          RES_BIT(1, E);
          DEBUG_PRINT("[INSTR] RES 1, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x8C:  // RES 1, H
          RES_BIT(1, H);
          DEBUG_PRINT("[INSTR] RES 1, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x8D:  // RES 1, L
          RES_BIT(1, L);
          DEBUG_PRINT("[INSTR] RES 1, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x8E:  // RES 1, (HL)
          RES_BIT(1, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 1, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x97:  // RES 2, A
          RES_BIT(2, A);
          DEBUG_PRINT("[INSTR] RES 2, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x90:  // RES 2, B
          RES_BIT(2, B);
          DEBUG_PRINT("[INSTR] RES 2, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x91:  // RES 2, C
          RES_BIT(2, C);
          DEBUG_PRINT("[INSTR] RES 2, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x92:  // RES 2, D
          RES_BIT(2, D);
          DEBUG_PRINT("[INSTR] RES 2, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x93:  // RES 2, E
          RES_BIT(2, E);
          DEBUG_PRINT("[INSTR] RES 2, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x94:  // RES 2, H
          RES_BIT(2, H);
          DEBUG_PRINT("[INSTR] RES 2, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x95:  // RES 2, L
          RES_BIT(2, L);
          DEBUG_PRINT("[INSTR] RES 2, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x96:  // RES 2, (HL)
          RES_BIT(2, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 2, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0x9F:  // RES 3, A
          RES_BIT(3, A);
          DEBUG_PRINT("[INSTR] RES 3, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x98:  // RES 3, B
          RES_BIT(3, B);
          DEBUG_PRINT("[INSTR] RES 3, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x99:  // RES 3, C
          RES_BIT(3, C);
          DEBUG_PRINT("[INSTR] RES 3, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x9A:  // RES 3, D
          RES_BIT(3, D);
          DEBUG_PRINT("[INSTR] RES 3, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x9B:  // RES 3, E
          RES_BIT(3, E);
          DEBUG_PRINT("[INSTR] RES 3, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x9C:  // RES 3, H
          RES_BIT(3, H);
          DEBUG_PRINT("[INSTR] RES 3, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x9D:  // RES 3, L
          RES_BIT(3, L);
          DEBUG_PRINT("[INSTR] RES 3, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0x9E:  // RES 3, (HL)
          RES_BIT(3, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 3, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xA7:  // RES 4, A
          RES_BIT(4, A);
          DEBUG_PRINT("[INSTR] RES 4, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA0:  // RES 4, B
          RES_BIT(4, B);
          DEBUG_PRINT("[INSTR] RES 4, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA1:  // RES 4, C
          RES_BIT(4, C);
          DEBUG_PRINT("[INSTR] RES 4, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA2:  // RES 4, D
          RES_BIT(4, D);
          DEBUG_PRINT("[INSTR] RES 4, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA3:  // RES 4, E
          RES_BIT(4, E);
          DEBUG_PRINT("[INSTR] RES 4, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA4:  // RES 4, H
          RES_BIT(4, H);
          DEBUG_PRINT("[INSTR] RES 4, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA5:  // RES 4, L
          RES_BIT(4, L);
          DEBUG_PRINT("[INSTR] RES 4, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA6:  // RES 4, (HL)
          RES_BIT(4, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 4, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xAF:  // RES 5, A
          RES_BIT(5, A);
          DEBUG_PRINT("[INSTR] RES 5, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA8:  // RES 5, B
          RES_BIT(5, B);
          DEBUG_PRINT("[INSTR] RES 5, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xA9:  // RES 5, C
          RES_BIT(5, C);
          DEBUG_PRINT("[INSTR] RES 5, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xAA:  // RES 5, D
          RES_BIT(5, D);
          DEBUG_PRINT("[INSTR] RES 5, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xAB:  // RES 5, E
          RES_BIT(5, E);
          DEBUG_PRINT("[INSTR] RES 5, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xAC:  // RES 5, H
          RES_BIT(5, H);
          DEBUG_PRINT("[INSTR] RES 5, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xAD:  // RES 5, L
          RES_BIT(5, L);
          DEBUG_PRINT("[INSTR] RES 5, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xAE:  // RES 5, (HL)
          RES_BIT(5, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 5, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xB7:  // RES 6, A
          RES_BIT(6, A);
          DEBUG_PRINT("[INSTR] RES 6, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB0:  // RES 6, B
          RES_BIT(6, B);
          DEBUG_PRINT("[INSTR] RES 6, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB1:  // RES 6, C
          RES_BIT(6, C);
          DEBUG_PRINT("[INSTR] RES 6, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB2:  // RES 6, D
          RES_BIT(6, D);
          DEBUG_PRINT("[INSTR] RES 6, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB3:  // RES 6, E
          RES_BIT(6, E);
          DEBUG_PRINT("[INSTR] RES 6, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB4:  // RES 6, H
          RES_BIT(6, H);
          DEBUG_PRINT("[INSTR] RES 6, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB5:  // RES 6, L
          RES_BIT(6, L);
          DEBUG_PRINT("[INSTR] RES 6, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB6:  // RES 6, (HL)
          RES_BIT(6, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 6, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;
        case 0xBF:  // RES 7, A
          RES_BIT(7, A);
          DEBUG_PRINT("[INSTR] RES 7, A\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB8:  // RES 7, B
          RES_BIT(7, B);
          DEBUG_PRINT("[INSTR] RES 7, B\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xB9:  // RES 7, C
          RES_BIT(7, C);
          DEBUG_PRINT("[INSTR] RES 7, C\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xBA:  // RES 7, D
          RES_BIT(7, D);
          DEBUG_PRINT("[INSTR] RES 7, D\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xBB:  // RES 7, E
          RES_BIT(7, E);
          DEBUG_PRINT("[INSTR] RES 7, E\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xBC:  // RES 7, H
          RES_BIT(7, H);
          DEBUG_PRINT("[INSTR] RES 7, H\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xBD:  // RES 7, L
          RES_BIT(7, L);
          DEBUG_PRINT("[INSTR] RES 7, L\n");
          ++*pc;
          *cycles = 8;
          break;
        case 0xBE:  // RES 7, (HL)
          RES_BIT(7, ram[HL]);
          DEBUG_PRINT("[INSTR] RES 7, (HL)\n");
          ++*pc;
          *cycles = 16;
          break;

        default:
          printf(
              "[ERROR] %s: Unkown extended instruction 0xCB 0x%02X at "
              "0x%04hX\n",
              __func__, opcode, *pc);
          return CPU_ERROR_UNK_INSTRUCTION;
      }
      break;
    default:
      printf("[ERROR] %s: Unkown instruction 0x%02X at 0x%04hX\n", __func__,
             opcode, *pc);
      return CPU_ERROR_UNK_INSTRUCTION;
  };

  DEBUG_PRINT(
      "AF: %04X BC: %04X DE: %04X HL: %04X SP: %04X "
      "%c%c%c%c\n",
      AF, BC, DE, HL, *sp, GET_Z ? 'Z' : '_', GET_N ? 'N' : '_',
      GET_H ? 'H' : '_', GET_C ? 'C' : '_');

  if (*pc == 0xFFFF) {
    dbg.trace = DBG_STEP;
  }

  if (*pc > 0xFF) {
    switch (opcode) {
      case 0xC9:
      case 0xC0:
      case 0xC8:
      case 0xD0:
      case 0xD8:
      case 0xD9:
      case 0xCD:
      case 0xC4:
      case 0xCC:
      case 0xD4:
      case 0xDC:
        if (dbg.trace == DBG_STEP_OVER) DebugTrace(&dbg);
    }

    if (dbg.trace == DBG_STEP) DebugTrace(&dbg);
  }

  return CPU_OK;
}

void DebugTrace(struct debug *dbg) {
  int c = getchar();
  switch (c) {
    case 'c':
    case 'C':
      dbg->trace = DBG_CONTINUE;
      break;
    case 'o':
    case 'O':
      dbg->trace = DBG_STEP_OVER;
      break;
    case 's':
    case 'S':
      dbg->trace = DBG_STEP;
      break;
  }
}

void DebugReadBlarggsSerial(uint8_t *ram) {
  if (ram[0xFF02] == 0x81) {  // IO busy
    char c = ram[0xFF01];     // read data
    printf("%c", c);
    ram[0xFF02] = 0x0;  // IO done
  }
}

void PrintBinary8(uint8_t u8) {
  printf("%d", (u8 & (1 << 7)) != 0);
  printf("%d", (u8 & (1 << 6)) != 0);
  printf("%d", (u8 & (1 << 5)) != 0);
  printf("%d", (u8 & (1 << 4)) != 0);
  printf("%d", (u8 & (1 << 3)) != 0);
  printf("%d", (u8 & (1 << 2)) != 0);
  printf("%d", (u8 & (1 << 1)) != 0);
  printf("%d ", (u8 & (1 << 0)) != 0);
}

void CoreDump(const char *fileName, uint8_t *ram) {
  if (!fileName || !ram) {
    return;
  }

  FILE *f = fopen(fileName, "w");
  fwrite(ram, 1, 0x10000, f);
  fclose(f);

  printf("[ERROR] Core dumped at %s\n", fileName);

  return;
}
