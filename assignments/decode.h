#ifndef DECODE_H
#define DECODE_H
#include "cpu_state.h"
#include "memory.h"
#include "typedefs.h"
#include "opCodes.h"
#include "reg_names.h"
#include "op_names.h"
#include <cassert>
#include <string>

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

// Printing functions
void print_decoded_nocalc(const char* instr, const char* dest, const char* source);
void print_decoded_source_calculated(const char* instr, const char* dest, const char* source, u32 N);
void print_decoded_destination_calculated(const char* instr, const char* dest, const char* source, u32 N);

// Handles instruction, updates cpu/register state
BYTES_HANDLED handle_mov_immediate(memory_segment, cpu_state&);


// Attempting to write a general functions and kneeding out some pattern that handles a good chunk of instructions
// Relies on the operation_names[], register_names[] and address_pointers[] to present the mnemonics in asm format. 
BYTES_HANDLED handle_general_register_memory(memory_segment memseg, cpu_state &state);

BYTES_HANDLED handle_general_memory(memory_segment memseg, cpu_state &state);

BYTES_HANDLED handle_general_immediate(memory_segment memseg, cpu_state &state);

BYTES_HANDLED handle_jump_instruction(memory_segment memseg, cpu_state& state);

#endif