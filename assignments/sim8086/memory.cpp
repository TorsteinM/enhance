#include "memory.h"


bool file_to_mem_cache(char* fname, memory_segment &memseg){
    ifstream ifile(fname, ios::binary);

    std::cout << "Reading from file: " << fname << std::endl;
    
    if(!ifile.is_open()) {
        std::cerr << "Couldn't open file." << std::endl;
        return false;
    }
        
    ifile.seekg(0, ios::end);
    int file_size = ifile.tellg();
    ifile.seekg(0, ios::beg);


    // Allocate the size of the file in memory
    char* mem = (char*)malloc(file_size);

    // Read the machine code into memory
    ifile.read(mem, file_size);

    memseg.memory = (uint8_t*)mem;


    // Check if the file was read successfully
    if (!ifile.good()) {
        cerr << "Error: Could not read file!" << endl;
        free(memseg.memory);
        ifile.close();
        return false;
    }

    // Finishing the memory object
    memseg.bytes_currently_read = file_size;
    memseg.current_offset = 0;

    // Close the file
    ifile.close();
    return true;

}