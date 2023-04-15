#ifndef DECODE_H
#define DECODE_H
#include "cpu_state.h"
#include "memory.h"
#include "typedefs.h"
#include "opCodes.h"
#include "reg_names.h"
#include <cassert>

enum BYTES_HANDLED{
    HANDLED_ZERO,
    HANDLED_ONE_BYTE,
    HANDLED_TWO_BYTES,
    HANDLED_THREE_BYTES,
    HANDLED_FOUR_BYTES,
    HANDLED_FIVE_BYTES,
    HANDLED_SIX_BYTES,
};

void decode_and_update_offset(memory_segment&, cpu_state&);

// Handles instruction, updates cpu/register state
BYTES_HANDLED handle_mov_immediate(memory_segment, cpu_state&);
// Handles instruction, updates cpu/register state
BYTES_HANDLED handle_mov_register(memory_segment, cpu_state&);



#endif