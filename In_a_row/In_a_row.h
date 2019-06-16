//
// Created by aditimishra on 5/31/2019.
//

#ifndef UNIPROCESSOR_IN_A_ROW_H
#define UNIPROCESSOR_IN_A_ROW_H

#include "../Instruction/Instruction.h"

typedef struct In_a_row{
    char *name_of_instruction_or_data;
    int type;               // it tells whether the address store instruction or data
    Instruction* instruction;   // address in the instruction points is an address CPU wants to read or write
    int data;               // if data, then only store some value in it otherwise it is -1
    Address* address;       // this is the address at which the instruction or data resides
} In_a_row;

In_a_row* initialize_each_column_of_main_memory(char *name_of_instruction_or_data, int type, Instruction* instruction, int data, Address* address);



#endif //UNIPROCESSOR_IN_A_ROW_H
