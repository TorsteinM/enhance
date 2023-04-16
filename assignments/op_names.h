#ifndef OP_NAMES_H
#define OP_NAMES_H

// Bitmask for operation name
#define OP_NAME_MASK 0b11111000

// Usage 1: Mask the first 5 bits if the instruction is lower than 0x60
// Usage 2: Mask 3 bits for [ADD,OR,ADC,SBB,AND,SUB,XOR,CMP] patterned instructions
// Usage 3: 0xD interval: Mask the 3 bit pattern and add 24 for [ROL,ROR,RCL,RCR,SAL/SHL,SHR,N/A,SAR] 
// If the name is "N/A" this structure is not suitable for the instruction,
// or it just isn't implemented yet.
extern const char* operation_names[32];

#endif