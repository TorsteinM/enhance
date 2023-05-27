#include "cpu_state.h"

void display_regs(cpu_state state){
    std::cout << "\n\n ______________________" << std::endl;
    std::cout << "|Register | Value (des)|" << std::endl;
    std::cout << "|_________|____________|" << std::endl;
    std::cout << "|   ax    | " << std::setfill(' ') << std::setw(6) <<  state.registers.ax << "     |" << std::endl;
    std::cout << "|   bx    | " << std::setfill(' ') << std::setw(6) <<  state.registers.bx << "     |" << std::endl;
    std::cout << "|   cx    | " << std::setfill(' ') << std::setw(6) <<  state.registers.cx << "     |" << std::endl;
    std::cout << "|   dx    | " << std::setfill(' ') << std::setw(6) <<  state.registers.dx << "     |" << std::endl;
    std::cout << "|   sp    | " << std::setfill(' ') << std::setw(6) <<  state.registers.sp << "     |" << std::endl;
    std::cout << "|   bp    | " << std::setfill(' ') << std::setw(6) <<  state.registers.bp << "     |" << std::endl;
    std::cout << "|   si    | " << std::setfill(' ') << std::setw(6) <<  state.registers.si << "     |" << std::endl;
    std::cout << "|   di    | " << std::setfill(' ') << std::setw(6) <<  state.registers.di << "     |" << std::endl;
    std::cout << "|_________|____________|" << std::endl;

}

void display_memory_5_lines(cpu_state state){
    int lines = 0;
    std::cout << "\n\n ______________________" << std::endl;
    std::cout << "|Memory   | Value (des)|" << std::endl;
    std::cout << "|_________|____________|" << std::endl;
    for (int i = 0; i < 4096; i++){
        if(state.memory[i] != 0){
            std::cout << "|   " << std::setfill('0') << std::setw(4) << i << "  | " << std::setfill(' ') << std::setw(6) <<  state.memory[i] << "     |" << std::endl;
            lines++;
            if(lines==8)
                break;         
        }
    }
    std::cout << "|_________|____________|" << std::endl;
}

// General comments on the functions:

// The simulate functions switch on operation enum. 
// This should work nicely for the twelve first types of instructions.[ADD, SUB, ... etc.]
// But the MOV instructions doesnt generally have the same structure and have various different codes.
// So these MOV instructions are handled by letting them have the initial code, and fall through to the same code in the switch statement.


// At the moment there are several simulate functions that duplicates alot of code. Hopefully there will be ways to generalize the paths.

void simulate_value_to_register(cpu_state &state, operation op, u32 reg, u32 value, bool wide){
    switch (op){
        case CMP:
            if(wide)  
                state.register_as_wides[reg] -= value;
            else
                state.register_as_bytes[reg] -= value;
            check_flags_general(state, reg, wide);
            return;
        case SUB:
            if (wide){
                std::cout << register_names[wide][reg] << " previous value: " << state.register_as_wides[reg];
                state.register_as_wides[reg] -= value;
                std::cout << ", new value: " << state.register_as_wides[reg] << std::endl;    
            } else {
                std::cout << register_names[wide][reg] << " previous value: " << state.register_as_bytes[reg];
                state.register_as_bytes[reg] -= value;
                std::cout << ", new value: " << state.register_as_bytes[reg] << std::endl;
            }
            check_flags_general(state, reg, wide);
            return;
        case POP:
            return;
        case MOV:
        case NR17:
            if (wide){
                std::cout << register_names[wide][reg] << " previous value: " << state.register_as_wides[reg];
                state.register_as_wides[reg] = value;
                std::cout << ", new value: " << state.register_as_wides[reg] << std::endl;
            } else {
                std::cout << register_names[wide][reg] << " previous value: " << state.register_as_bytes[reg];
                state.register_as_bytes[reg] = value;
                std::cout << ", new value: " << state.register_as_bytes[reg] << std::endl;
            }
            return;
        default:
            std::cerr << "Some operation that isnt implemented was encountered. Doing Nothing." << std::endl;
            return;
    }
}

void simulate_value_to_memory(cpu_state &state, u32 value, u32 address, bool wide){
    if(wide){
        // High and low part
        uint8_t high = (value >> 8) & 0xFF;
        uint8_t low = value & 0xFF;
        // Write out previous value
        std::cout << "Memory value: " << state.memory[address] << std::endl;
        // Low part into memory
        state.memory[address] = low;
        // High part into memory
        state.memory[address+1] = high;
        // Write out new value
        std::cout << "Memory value: " << state.memory[address] << std::endl;

    } else {
        std::cout << "Memory value: " << state.memory[address] << std::endl;
        state.memory[address] = value;
        std::cout << "Memory value: " << state.memory[address] << std::endl;
    }
}

uint32_t simulate_value_from_memory(cpu_state &state, u32 address, bool wide){
    if(wide){
        std::cout << "Memory value: " << state.memory[address] << std::endl;
        return state.memory[address];
    } else {
        std::cout << "Memory value: " << state.memory[address] << std::endl;
        return state.memory[address];
    }
}
// Maybe this will work. Just getting the value from source and passing it to the function. Keeping the function for now in case some special case.

void simulate_register_to_register(cpu_state &state, operation op, u32 destination_reg, u32 source_reg, bool wide){
    
    u32 source_value = wide ? state.register_as_wides[source_reg] : state.register_as_bytes[source_reg];
    simulate_value_to_register(state, op, destination_reg, source_value, wide);

}



void check_flags_general(cpu_state &state, u32 reg, bool wide){
    if(wide){
        if(state.flags.zf != !(state.register_as_wides[reg])){
            state.flags.zf = !(state.register_as_wides[reg]);
            if(state.flags.zf){
                std::cout << "Zero flag was set." << std::endl;
            } else {
                std::cout << "Zero flag was cleared." << std::endl;
            }
        }

        if(state.flags.sf != (state.register_as_wides[reg] >> 15)){
            state.flags.sf = (state.register_as_wides[reg] >> 15);
            if(state.flags.sf){
                std::cout << "Sign flag was set." << std::endl;
            } else {
                std::cout << "Sign flag was cleared." << std::endl;
            }
        }
    } else {
        if(state.flags.zf != !(state.register_as_bytes[reg])){
            state.flags.zf = !(state.register_as_bytes[reg]);
            if(state.flags.zf){
                std::cout << "Zero flag was set." << std::endl;
            } else {
                std::cout << "Zero flag was cleared." << std::endl;
            }
        }

        if(state.flags.sf != (state.register_as_bytes[reg] >> 7)){
            state.flags.sf = (state.register_as_bytes[reg] >> 7);
            if(state.flags.zf){
                std::cout << "Sign flag was set." << std::endl;
            } else {
                std::cout << "Sign flag was cleared." << std::endl;
            }
        }
    }
}