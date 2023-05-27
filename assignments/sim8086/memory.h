#ifndef MEMORY_H
#define MEMORY_H

#include "typedefs.h"
#include <fstream>
#include <iostream>


using namespace std;

// Struct for memory segment
// Holds pointer to memory, where we are currently reading and how many bytes are precent in the memory.
struct memory_segment {
    uint8_t* memory = NULL;
    u32 current_offset = 0;
    u32 bytes_currently_read = 0;
};

// Function that allocates memory for a segment and reads a file into the memory.
// If returned true, the memory_segment object is finished and it's memory must be freed later.
bool file_to_mem_cache(char *, memory_segment&);

#endif