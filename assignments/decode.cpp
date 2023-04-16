#include "decode.h"
#include <iostream>


void print_decoded_nocalc(const char* instr, const char* dest, const char* source){
    std::cout << instr << " " << dest << ", " << source << std::endl;
}
void print_decoded_source_calculated(const char* instr, const char* dest, const char* source, u32 N){
    if (N)
        std::cout << instr << " " << dest << ", [" << source << " + " << N << "]" << std::endl;
    else
        std::cout << instr << " " << dest << ", [" << source << "]" << std::endl;
}
void print_decoded_destination_calculated(const char* instr, const char* dest, const char* source, u32 N){
    if (N)
        std::cout << instr << " [" << dest << " + " << N << "], " << source << std::endl;
    else
        std::cout << instr << " [" << dest << "], " << source << std::endl;
}

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
        std::cout << "MOV " << register_names[1][reg] << ", " << ((byte3 << 8) + byte2) << std::endl;
        update_value_to_register(state, (operation)12, reg, (byte3 << 8) + byte2, true);
        return HANDLED_THREE_BYTES;
    // Handle 8bit immediate to register.
    } else {
        std::cout << "MOV " << register_names[0][reg] << ", " << (int)byte2 << std::endl;
        update_value_to_register(state, (operation)12, reg, byte2, false);
        return HANDLED_TWO_BYTES;
    }
}

BYTES_HANDLED handle_general_register_memory(memory_segment memseg, cpu_state &state){
    
    uint8_t byte1 = *(memseg.memory + memseg.current_offset);
    uint8_t byte2 = *(memseg.memory + memseg.current_offset + 1);
    uint8_t byte3 = *(memseg.memory + memseg.current_offset + 2);
    uint8_t byte4 = *(memseg.memory + memseg.current_offset + 3);
    
    bool reg_is_dest = byte1 & D_BIT;
    bool wide = byte1 & W_BIT;
    u32 reg = (byte2 & REG) >> 3;
    u32 rm = byte2 & RM;

    u32 opcode = (byte1 & OP_NAME_MASK) >> 3;

    // std::cout << "REG: " << reg << std::endl;
    // std::cout << "RM: " << rm << std::endl;
    // Control structure for the 4 different MODES 
    switch (byte2 & MOD){
        case MOD_NO_DISP:
            if(reg_is_dest)
                print_decoded_source_calculated(operation_names[opcode], register_names[wide][reg], address_pointers[rm], 0);
            else
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], register_names[wide][reg], 0);
            return HANDLED_TWO_BYTES;
            break;
        case MOD_8BIT:

            if(reg_is_dest)
                print_decoded_source_calculated(operation_names[opcode], register_names[wide][reg], address_pointers[rm], byte3);
            else
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], register_names[wide][reg], byte3);
            return HANDLED_THREE_BYTES;
            break;
        case MOD_16BIT:
            if(reg_is_dest)
                print_decoded_source_calculated(operation_names[opcode], register_names[wide][reg], address_pointers[rm], (byte4 << 8) + byte3);
            else
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], register_names[wide][reg], (byte4 << 8) + byte3);
            return HANDLED_FOUR_BYTES;
            break;
        case MOD_REGISTER:
            // TODO: Something had broken here. Maybe it was edited out of turn.
            if (true) {
                if (reg_is_dest)
                    print_decoded_nocalc(operation_names[opcode], register_names[wide][reg], register_names[wide][rm]);
                else
                    print_decoded_nocalc(operation_names[opcode], register_names[wide][rm], register_names[wide][reg]);

                return HANDLED_TWO_BYTES;
            }
            else {
                if (reg_is_dest)
                    print_decoded_nocalc(operation_names[opcode], register_names[wide][reg], address_pointers[rm]);
                else
                    print_decoded_nocalc(operation_names[opcode], address_pointers[rm], register_names[wide][reg]);

                return HANDLED_TWO_BYTES;
            }
            
            break;
        default:
            std::cerr << "somehow happened upon default" << std::endl;
            return HANDLED_ZERO;
            break;
    }
}

BYTES_HANDLED handle_general_immediate(memory_segment memseg, cpu_state &state){
    // Converting memory. Reading the 3 byte aswell, even tho it might not be used.
    // Accessing memory twice half the time will most likely be slower than reading an extra byte half the time 
    uint8_t byte1 = *(memseg.memory + memseg.current_offset);
    uint8_t byte2 = *(memseg.memory + memseg.current_offset + 1);
    uint8_t byte3 = *(memseg.memory + memseg.current_offset + 2);
    // Using RM mask to extract 3 last bits.

    bool wide = byte1 & W_BIT;
    u32 opcode = (byte1 & OP_3BIT_MASK) >> 3;
    u32 reg = 0;

    // Handle 16bit immediate to register.
    if(wide){
        print_decoded_nocalc(operation_names[opcode], register_names[wide][reg], std::to_string((byte3 << 8) + byte2).c_str());
        return HANDLED_THREE_BYTES;
    // Handle 8bit immediate to register.
    } else {
        print_decoded_nocalc(operation_names[opcode], register_names[wide][reg], std::to_string(byte2).c_str());
        return HANDLED_TWO_BYTES;
    }
}

BYTES_HANDLED handle_general_memory(memory_segment memseg, cpu_state &state){
    
    uint8_t byte1 = *(memseg.memory + memseg.current_offset);
    uint8_t byte2 = *(memseg.memory + memseg.current_offset + 1);
    uint8_t byte3 = *(memseg.memory + memseg.current_offset + 2);
    uint8_t byte4 = *(memseg.memory + memseg.current_offset + 3);
    uint8_t byte5 = *(memseg.memory + memseg.current_offset + 4);
    uint8_t byte6 = *(memseg.memory + memseg.current_offset + 5);
    

    // TODO:
    // 0x80-0x83 seems strange and might be best switched on the last two bits
    // However, it still needs to be switched on the MOD field. 
    // Which gives 16 cases in total.


    // Sign extension not implemented yet
    bool sign_extended = byte1 & SIGN_EXT_BIT;
    bool wide = byte1 & W_BIT;
    // the register is in the RM-field
    u32 rm = byte2 & RM;
    // the instruction is in the REG-field
    u32 opcode = (byte2 & REG) >> 3;

    switch (byte2 & MOD){
        case MOD_NO_DISP:
            // TODO: Hardcoding a bothersome instruction for now (it says 00 in the MOD-field even though it is 16bit). 
            if (byte1 == 0x83 && byte2 == 0x3e) {
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string(byte5).c_str(), (byte4 << 8) + byte3);
                return HANDLED_FIVE_BYTES;
            }
            if(wide && !sign_extended){
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string((byte4 << 8) + byte3).c_str(), 0);
                return HANDLED_FOUR_BYTES;
            } else {
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string(byte3).c_str(), 0);
                return HANDLED_THREE_BYTES;
            }
        case MOD_8BIT:    
            if(wide && !sign_extended){
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string((byte5 << 8) + byte4).c_str(), byte3);
                return HANDLED_FIVE_BYTES;
            } else {
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string(byte4).c_str(), byte3);
                return HANDLED_FOUR_BYTES;
            }
        case MOD_16BIT:
            //Hack for sign + wide instruction
            //if (wide & sign_extended) {
            //    print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string(byte5).c_str(), (byte4 << 8) + byte3);
            //    return HANDLED_FIVE_BYTES;
            //}
            if(wide && !sign_extended){
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string((byte6 << 8) + byte5).c_str(), (byte4 << 8) + byte3);
                return HANDLED_SIX_BYTES;
            } else {
                print_decoded_destination_calculated(operation_names[opcode], address_pointers[rm], std::to_string(byte5).c_str(), (byte4 << 8) + byte3);
                return HANDLED_FIVE_BYTES;
            }
        case MOD_REGISTER:
            std::cerr << "MOD-REGISTER NEEDS WORK IF ITS TO BE USED." << std::endl;
            if(wide)
                print_decoded_nocalc(operation_names[opcode], register_names[wide][rm], std::to_string(byte3).c_str());
            else 
                print_decoded_nocalc(operation_names[opcode], std::to_string(byte3).c_str(), register_names[wide][rm]);
            return HANDLED_THREE_BYTES;
        default:
            std::cerr << "MODE not implemented yet, byte2: " << byte2 << std::endl;
            return HANDLED_ZERO;
    }
}

BYTES_HANDLED handle_jump_instruction(memory_segment memseg, cpu_state& state) {
    // Jumping over this atm
    return HANDLED_TWO_BYTES;
}


void decode_and_update_offset(memory_segment &memseg, cpu_state &state){
    
    auto current_byte = *(memseg.memory + memseg.current_offset);
    uint8_t left_four_bits = current_byte & LEFT;
    BYTES_HANDLED bytes_processed = HANDLED_ZERO;
    
    // std::cout << "Byte: " << (int)current_byte << std::endl;
    // std::cout << "LEFT: " << (int)left_four_bits << std:: endl;
    
    // Switch statement based on the leftmost 4 bits in the instruction code.
    // There is still need for alot of control in each of the intervals, 
    // but hopefully not much overlap of instructions across the intervals
    switch (left_four_bits){
        case 0x00:            
        case 0x10:
        case 0x20:
        case 0x30:
            // Grouping all instructions < 0x40 together. 
            // TODO: Consider moving the MODE switching here
            if((current_byte & IMM_MASK) == IMM_INSTRUCTION ){
                bytes_processed = handle_general_immediate(memseg, state);
            } else {
                bytes_processed = handle_general_register_memory(memseg, state);
            }
            
            memseg.current_offset += bytes_processed;
            return; // returning directly instead of breaking
        
        case 0x40:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
       
        case 0x50:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0x60:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0x70:
            // We 'bout to jump
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0x80:
            // 
            if(current_byte < 0x84 ){
                // Does not have a reg field, instead the instruction is coded in the REG field.
                bytes_processed = handle_general_memory(memseg, state);
                memseg.current_offset += bytes_processed;

            } else if(current_byte < 0x8D){
                // std::cout << "Entering OPCODE for MOV_REGISTER" << std::endl;
                bytes_processed = handle_general_register_memory(memseg, state);
                // std::cout << "Bytes processed: " << bytes_processed << std::endl;
                memseg.current_offset += bytes_processed;
            } else {
                std::cerr << "Not implemented yet.\nAborting" << std::endl;
                // Forcing end of decode
                memseg.current_offset = memseg.bytes_currently_read;
            }
            return;
        
        case 0x90:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0xA0:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0xB0:
            // Handle immediate value to register
            bytes_processed = handle_mov_immediate(memseg, state);
            // std::cout << "Bytes processed in immediate to register: " << bytes_processed << std::endl;
            memseg.current_offset += bytes_processed;
            return; // returning directly instead of breaking
        
        case 0xC0:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0xD0:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0xE0:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
        
        case 0xF0:
            std::cerr << "Not implemented yet.\nAborting" << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
       
        default:
            std::cerr << "Shouldn't reach this point, something is wrong." << std::endl;
            // Forcing end of decode
            memseg.current_offset = memseg.bytes_currently_read;
            return; // returning directly instead of breaking
    }
}
