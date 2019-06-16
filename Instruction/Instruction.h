//
// Created by aditimishra on 5/31/2019.
//

#ifndef UNIPROCESSOR_INSTRUCTION_H
#define UNIPROCESSOR_INSTRUCTION_H


#include "../Address/Address.h"

typedef struct Instruction {
    char *name_of_instruction_or_data;
    int type_of_instruction;  // 10 for CPURead and 20 for CPUWrite
    int address;              // this is the address CPU want to read or write
    int number_of_bytes;    // number of bytes cpu wants to read or write
    int value;              // if it is CPUWrite operation then what is the changed value
} Instruction;

Instruction* initialize_instruction(char *name_of_instruction_or_data,int type_of_instruction, int address, int number_of_bytes, int value);


#endif //UNIPROCESSOR_INSTRUCTION_H
