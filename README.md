# enhance
Following CMuratori's computer enhance course

# The aim of the repository:
To share and have some incentive to make something decent and with readable code.

The assignments for the first part of the course is pretty much to emulate a handful of 8086 CPU instructions. As such the code is written basicly with the aim of creating a simple 8086 emulator.

The biggest tool for accomplishing this is the 8086 User's Manual:
https://edge.edx.org/c4x/BITSPilani/EEE231/asset/8086_family_Users_Manual_1_.pdf

# Style of coding - Playground/No principal scheme
Being somewhat influenced by the course creator the code is not designed around classes and methods.

There is some aim to keep it structured and learn how to do this properly in C/C++ by trying to code it by common sense.

# CMake - only basic options
Only one CMakeLists.txt is used, which builds all the source files into one executable.

# Files and types
For now the code has it's state contained within two structs:
- CPU instruction cache:
    - Memory
    - Current offset 
    - Total bytes 
- CPU state
    - 8 registers that is 16bit wide.
    - The first 4 registers are defined as unions as they can be subadressed by the 8086 CPU
    - 8 flags of the CPU.
    
# Tasks in main
1. User interaction
2. Create and init CPU state
3. Creating Instruction cache and loading instructions
4. Decoding the instructions in the cache
5. Cleaning up

  
