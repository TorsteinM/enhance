#include "cpu_state.h"

void display_regs(cpu_state state){
    auto ptr = &state.registers.ax;
    for (int i = 0; i < REGISTER_LENGTH; i++){
        std::cout << register_names[1][i] << " " << *(ptr+i) << std::endl;
    }
}

void update_value_to_register(cpu_state &state, operation op, const u32 reg, u32 value, bool wide){
    switch (op){
        case POP:
            return;
        case MOV:
            if (wide){
                std::cout << "Previous value: " << state.register_as_wides[reg];
                state.register_as_wides[reg] = value;
                std::cout << ", new value: " << state.register_as_wides[reg] << std::endl;
            } else {
                std::cout << "Previous value: " << state.register_as_bytes[reg];
                state.register_as_bytes[reg] = value;
                std::cout << ", new value: " << state.register_as_bytes[reg] << std::endl;
            }
            return;
        default:
            std::cerr << "Some operation that isnt implemented was encountered. Doing Nothing." << std::endl;
            return;
    }
}