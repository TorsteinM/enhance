#ifndef MAIN_H
#define MAIN_H
#include "typedefs.h"


#endif


/*
    The CPU has some memory that it reads its instructions from.

    The memory needs to be read to simulate 8086 instructions.

    Either we can read byte for byte from the file, or we can load it into memory. 
    The latter approach seems to be the closest to what a processor actually does.
    
    To do this we need need to make a routine that does it:
        - The routine needs to be passed the memory struct to keep track of it
        - How much to read?? Not sure.
        - Return value? Not sure if needed.
        - Current offset should be set to zero.
        - Maybe just call it populate memory?

    The 8086 has variable instruction length, that means that it is important to know where to start and how much to read.



    The CPU has registers

*/