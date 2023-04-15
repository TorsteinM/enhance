# enhance
Following CMuratori's computer enhance course

# The aim of the repository:
To use the whole toolchain and have some incentive to make something decent and with readable code.

# Style of coding - Playground/No principal scheme
Being somewhat influenced by the course creator the code is not designed around classes and methods.

There is some aim to keep it structured and learn how to do this properly in C/C++ by trying to code it by common sense.

# CMake - only basic options
Only one CMakeLists.txt is used, which builds all the source files into one executable.

# Files and types
For now it is mostly built around two structs:
- One that holds the instruction cache of the CPU:
    - Memory
    - Current offset 
    - Total bytes 
- Another struct that holds the register and flag state of CPU
    - 8 registers that is 16bit wide.
    - The CPU can address the first 4 as a wide(x), or as high(h) and low(l).
    - 8 flags are also present in the CPU.
    - This whole struct is initialized with zeros at the beginning of main.
    
# Tasks in main
1. User interaction
2. Creating CPU state
3. Creating Instruction cache and loading instructions
4. Decoding the instructions in the cache
5. Cleaning up

  
