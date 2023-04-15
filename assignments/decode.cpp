#include "decode.h"
#include <iostream>


BYTES_HANDLED handle_mov_immediate(memory_segment memseg, cpu_state &state){
    // Converting memory. Reading the 3 byte aswell, even tho it might not be used.
    // Accessing memory twice half the time will most likely be slower than reading an extra byte half the time 
    uint8_t byte1 = *(memseg.memory + memseg.current_offset);
    uint8_t byte2 = *(memseg.memory + memseg.current_offset + 1);
    uint8_t byte3 = *(memseg.memory + memseg.current_offset + 2);
    // Using RM mask to extract 3 last bits.
    uint8_t reg = byte1 & RM;

    // Handle 16bit immediate to register.
    if(byte1 & MOV_IMM_WITDH){
        std::cout << "MOV " << register_names[1][reg] << ", " << (int)((byte3 << 8) + byte2) << std::endl;
        return HANDLED_THREE_BYTES;
    // Handle 8bit immediate to register.
    } else {
        std::cout << "MOV " << register_names[0][reg] << ", " << (int)byte2 << std::endl;
        return HANDLED_TWO_BYTES;
    }
}

BYTES_HANDLED handle_mov_register(memory_segment memseg, cpu_state &state){
    
    uint8_t byte1 = *(memseg.memory + memseg.current_offset);
    uint8_t byte2 = *(memseg.memory + memseg.current_offset + 1);
    uint8_t byte3 = *(memseg.memory + memseg.current_offset + 2);
    uint8_t byte4 = *(memseg.memory + memseg.current_offset + 3);
    
    bool reg_is_dest = byte1 & D_BIT;
    bool wide = byte1 & W_BIT;
    u32 reg = (byte2 & REG) >> 3;
    u32 rm = byte2 & RM;

    // std::cout << "REG: " << reg << std::endl;
    // std::cout << "RM: " << rm << std::endl;
    // Control structure for the 4 different MODES 
    switch (byte2 & MOD){
        case MOD_NO_DISP:
            if(reg_is_dest){
                std::cout << "mov " << register_names[wide][reg] << ", [" << address_calculation[rm] << "]" << std::endl;
            } else {
                std::cout << "mov " << "["  << address_calculation[rm]  << "], " << register_names[wide][reg]<< std::endl;
            }
            return HANDLED_TWO_BYTES;
            break;
        case MOD_8BIT:
            if(reg_is_dest){
                std::cout << "mov " << register_names[wide][reg] << ", [" 
                << address_calculation[rm] << " + " << (int)byte3 << "]" << std::endl;
            } else {
                std::cout << "mov " << "["  << address_calculation[rm]  
                << " + " << byte3 << "], " << register_names[wide][reg]<< std::endl;
            }
            return HANDLED_THREE_BYTES;
            break;
        case MOD_16BIT:
            if(reg_is_dest){
                std::cout << "mov " << register_names[wide][reg] << ", [" 
                << address_calculation[rm] << " + " << (byte4 << 8) + byte3 << "]" << std::endl;
            } else {
                std::cout << "mov " << "["  << address_calculation[rm]  
                << " + " << (byte4 << 8) + byte3 << "], " << register_names[wide][reg]<< std::endl;
            }
            return HANDLED_FOUR_BYTES;
            break;
        case MOD_REGISTER:
            if(reg_is_dest){
                std::cout << "MOV " << register_names[wide][reg] << ", " << register_names[wide][rm] << std::endl;
            } else {
                std::cout << "MOV " << register_names[wide][rm] << ", " << register_names[wide][reg] << std::endl;
            }
            return HANDLED_TWO_BYTES;
            break;
        default:
            cout << "somehow happened upon default" << endl;
            return HANDLED_ZERO;
            break;
    }
}

void decode_and_update_offset(memory_segment &memseg, cpu_state &state){
    auto current_byte = *(memseg.memory + memseg.current_offset);
    BYTES_HANDLED bytes_processed = HANDLED_ZERO;
    
    uint8_t left_four_bits = current_byte & LEFT;
    
    
    // std::cout << "Byte: " << (int)current_byte << std::endl;
    // std::cout << "LEFT: " << (int)left_four_bits << std:: endl;


    // Switch statement based on the leftmost 4 bits in the instruction code.
    // There is still need for alot of control in each of the intervals, 
    // but hopefully not much overlap of instructions across the intervals
    switch (left_four_bits){
        case 0x00:
            break;
        
        case 0x10:
            break;
        
        case 0x20:
            break;
        
        case 0x30:
            break;
        
        case 0x40:
            break;
       
        case 0x50:
            break;
        
        case 0x60:
            break;
        
        case 0x70:
            break;
        
        case 0x80:
            
            if(current_byte | OPCODE == MOV_REGISTER){
                // std::cout << "Entering OPCODE for MOV_REGISTER" << std::endl;
                bytes_processed = handle_mov_register(memseg, state);
                // std::cout << "Bytes processed: " << bytes_processed << std::endl;
                memseg.current_offset += bytes_processed;
            }
            break;
        
        case 0x90:
            break;
        
        case 0xA0:
            break;
        
        case 0xB0:
            // Handle immediate value to register
            bytes_processed = handle_mov_immediate(memseg, state);
            // std::cout << "Bytes processed in immediate to register: " << bytes_processed << std::endl;
            memseg.current_offset += bytes_processed;
            break;
        
        case 0xC0:
            assert(bytes_processed != HANDLED_ZERO);
            break;
        
        case 0xD0:
            assert(bytes_processed != HANDLED_ZERO);
            break;
        
        case 0xE0:
            assert(bytes_processed != HANDLED_ZERO);
            break;
        
        case 0xF0:
            assert(bytes_processed != HANDLED_ZERO);
            break;
       
        default:
            std::cerr << "Shouldn't reach this point, something is wrong." << std::endl;
            assert(NULL);
            break;

    }
}
