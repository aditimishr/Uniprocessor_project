//
// Created by aditimishra on 5/31/2019.
//

#include "In_a_row.h"
#include "malloc.h"

In_a_row* initialize_each_column_of_main_memory(char *name_of_instruction_or_data, int type, Instruction* instruction, int data, Address* address){
    In_a_row* inARow = malloc(sizeof(In_a_row));
    inARow -> name_of_instruction_or_data = name_of_instruction_or_data;
    inARow -> type = type;
    inARow -> instruction = instruction;
    inARow -> data = data;
    inARow -> address = address;
}
