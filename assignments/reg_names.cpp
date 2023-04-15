#include "reg_names.h"

const char* register_names[REGISTER_DEPTH][REGISTER_LENGTH] = {
    {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"}, 
    {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"}
    };
const char* address_calculation[REGISTER_LENGTH] = 
    {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"};
