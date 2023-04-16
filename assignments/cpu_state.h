
#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <stdint.h>
#include <iostream>
#include "typedefs.h"
#include "reg_names.h"


// Register structure
typedef struct {
    union {
        uint16_t ax;
        struct {
            uint8_t al;
            uint8_t ah;
        };
    };
    union {
        uint16_t cx;
        struct {
            uint8_t cl;
            uint8_t ch;
        };
    };
    union {
        uint16_t dx;
        struct {
            uint8_t dl;
            uint8_t dh;
        };
    };
    union {
        uint16_t bx;
        struct {
            uint8_t bl;
            uint8_t bh;
        };
    };
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
} cpu_state_t;

// Flag structure
typedef struct {
    uint8_t cf;
    uint8_t pf;
    uint8_t af;
    uint8_t zf;
    uint8_t sf;
    uint8_t tf;
    uint8_t iflag;
    uint8_t df;
    uint8_t of;
} flags_t;


struct cpu_state {
    union{
        cpu_state_t registers;
        int16_t register_as_wides[8];
        int8_t register_as_bytes[16];
    };
    
    flags_t flags;
};

enum operation{ 
    ADD,  //  0
    OR,   //  1
    ADC,  //  2
    SBB,  //  3
    AND,  //  4
    SUB,  //  5
    XOR,  //  6
    CMP,  //  7
    INC,  //  8
    DEC,  //  9
    PUSH, // 10
    POP,  // 11
    MOV,  // 12
    NR13,     // 13
    NR14,     // 14
    NR15,     // 15
    NR16,     // 16
    NR17,     // 17
    NR18,     // 18
    NR19,     // 19
    NR20,     // 20
    NR21,     // 21
    NR22,     // 22
    NR23,     // 23
    ROL,     // 24
    ROR,     // 25
    RCL,     // 26
    RCR,     // 27
    SAL_SHL,     // 28
    SHR,     // 29
    NR30,     // 30
    SAR     // 31 
    };


void display_regs(cpu_state);
void update_value_to_register(cpu_state &, operation, u32, u32, bool);
#endif // CPU_STATE_H
