
#include <fstream>
#include <iostream>
#include "main.h"
#include "memory.h"
#include "cpu_state.h"
#include "decode.h"



int main(int argc, char* argv[]){
    // 1. Inform user how the program should be used.
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <8086 machine code>" << std::endl;
        return EXIT_FAILURE;
    }

    // 2. Create CPU state
    cpu_state state_8086 = {0};

    // Allocating 64KB of memory for the CPU
    state_8086.memory = (uint16_t*)calloc(32768, sizeof(uint16_t));
    if(state_8086.memory == NULL){
        std::cerr << "Could not allocate memory for the CPU.\nAborting..." << std::endl;
        return EXIT_FAILURE;
    }

    //display_regs(state_8086);

    // 3. Allocate instruction cache and read instructions into it.
    memory_segment instruction_cache;

    if(!file_to_mem_cache(argv[1], instruction_cache)){
        std::cerr << "Instruction cache not created or loaded properly.\nAborting..." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Read " << instruction_cache.bytes_currently_read << " bytes of machine code into memory." << std::endl;

    // 4. Executing the instructions in memory
    while(instruction_cache.current_offset < instruction_cache.bytes_currently_read){
        //std::cout << "Handling instruction at offset: " << instruction_cache.current_offset << std::endl; 
        decode_and_update_offset(instruction_cache, state_8086);
    }

    // Displaying registers after everything is done
    display_regs(state_8086);
    display_memory_5_lines(state_8086);
    // 5. Cleaning up
    free(instruction_cache.memory);
    free(state_8086.memory);

    return EXIT_SUCCESS;
}