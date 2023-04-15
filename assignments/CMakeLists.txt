cmake_minimum_required(VERSION 3.15)

project(comp_enhance CXX C ASM)

add_executable(sim86 main.cpp memory.cpp cpu_state.cpp decode.cpp reg_names.cpp)

