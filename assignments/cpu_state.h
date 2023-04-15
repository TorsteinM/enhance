
#ifndef CPU_STATE_H
#define CPU_STATE_H

#include <stdint.h>
#include <iostream>

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
    cpu_state_t registers;
    flags_t flags;
};


void display_regs(cpu_state);

#endif // CPU_STATE_H
