# enhance
Following CMuratori's computer enhance course

# The aim of the repository
To practice building software generally and how to interface hardware in a practical way with readable code.  

To share and have some incentive to make something decent and structured.

# Part 2: haversine
So far there is code to generate random points on a sphere and using a reference function to calculate the difference between them.

## Generating sphere points
There are two methods implemented to generate random points on a sphere. One is the naiv implementation of creating two random angles for latitude and longitude, while the other one takes a clustered approach.

The need for a different approach than naive uniform angles is so that the average distance between points doesn't converge to some value. 

# Part 1: sim8086
The assignments for the first part of the course is pretty much to emulate a handful of 8086 CPU instructions.

As such the code is written to read 8086 machine code, decode instructions and simulate what the processor would do with them.

The biggest tool for accomplishing this is the 8086 User's Manual:
https://edge.edx.org/c4x/BITSPilani/EEE231/asset/8086_family_Users_Manual_1_.pdf

## Coding style
Being somewhat influenced by the course creator the code is not designed around classes and methods.

There is some aim to keep it structured and learn how to do this properly in C/C++ by trying to code it by common sense.

Common sense would be to keep the code readable, modular and trying to avoid side effects in functions.

It is hard to avoid repeating code entirely, but hopefully some patterns will emerge where repetitions seem to be unavoidable.

## Poor integration during first implementation
The nature of incremental assignments on something that builds to something larger, together with my blind spots and misconceptions, lead to poor integration of the workings of a 8086. 

A nice experience to see the lack of precision creeping in, making it progressively harder to reason about implementing new functionality on top of a more or less disintegrated structure.

The last assignments for part 1 aren't implemented.

## Files and types - Needs a rework.
For now the code has it's state contained within two structs:
- CPU instruction cache:
    - Memory
    - Current offset 
    - Total bytes 
- CPU state
    - 8 registers that is 16bit wide.
    - The first 4 registers are defined as unions as they can be subadressed by the 8086 CPU
    - 8 flags of the CPU.

The instructions themselves could be written as objects, but as it's a project that is growing they are handled directly. Making them into objects at some point would be preferable, to get all the benefits of having them read serially and h
    
## Tasks in main
1. User interaction
2. Create and init CPU state
3. Creating Instruction cache and loading instructions
4. Decoding and executing the instructions in the cache
5. Cleaning up

  
