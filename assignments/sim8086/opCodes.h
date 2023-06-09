#ifndef OPCODES_H
#define OPCODES_H

// BITMASK for MOD-field
#define MOD     0b11000000
// BITMASK for REG-field
#define REG     0b00111000
// BITMASK for RM-field
#define RM      0b00000111
// BITMASK for opcode
#define OPCODE  0b11111100
// BITMASK for Direction Bit
#define D_BIT   0b00000010
// BITMASK for Width Bit
#define W_BIT   0b00000001
// BITMASK for LEFT 4 bits
#define LEFT    0b11110000
// BITMASK for MOV_IMM_WITDH
#define MOV_IMM_WITDH 0b00001000
// BITMASK for MOV_IMM_WITDH
#define MOV_IMM_REG 0b00000111
// BITMASK for SIGN Extension
#define SIGN_EXT_BIT 0b00000010
// BITMASK for immediate instruction
#define IMM_MASK     0b00000110

// Field for control
#define IMM_INSTRUCTION 0b00000100

// INSTRUCTIONS ON 4 first BITS
#define MOV_IMMIDIATE 0b10110000
#define MOV_REGISTER  0b10001000
#define MOV_MEMORY    0b11000100


// MOD FIELDS
// MEMORY_MODE NODISPLACEMENT
#define MOD_NO_DISP  0b00000000
#define MOD_8BIT     0b01000000
#define MOD_16BIT    0b10000000
#define MOD_REGISTER 0b11000000

#endif