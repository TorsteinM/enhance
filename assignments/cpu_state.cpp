#include "cpu_state.h"

void display_regs(cpu_state state){
    auto ptr = &state.registers.ax;
    for (int i = 0; i < REGISTER_LENGTH; i++){
        std::cout << register_names[1][i] << " " << *(ptr+i) << std::endl;
    }
}
