//
// Created by aditimishra on 5/31/2019.
//
#include "malloc.h"
#include "Instruction.h"

Instruction* initialize_instruction(char * name_of_instruction, int type_of_instruction, int address, int number_of_bytes, int value){
    Instruction* instruction = malloc(sizeof(Instruction));
    instruction -> name_of_instruction_or_data = name_of_instruction;
    instruction -> type_of_instruction = type_of_instruction;
    instruction -> address = address;
    instruction -> number_of_bytes = number_of_bytes;
    if(instruction -> type_of_instruction == 10){
        instruction -> value = -1;
    }
    else {
        instruction->value = value;
    }
}

