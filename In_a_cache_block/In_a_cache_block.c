//
// Created by aditimishra on 5/30/2019.
//

#include "In_a_cache_block.h"
#include "malloc.h"
#include "stdio.h"

In_a_block* inititialize_with_instruction_or_data_or_both(Address* address, int type_of_cache){
    In_a_block* instruction_or_data = malloc(sizeof(In_a_block));
    instruction_or_data -> address = address;
    instruction_or_data -> type_of_cache = type_of_cache;
    instruction_or_data -> name_of_instruction_or_data = NULL;

    if(instruction_or_data -> type_of_cache == 0){ // for L1 instruction cache, assign instruction = 0 and data = -1
        instruction_or_data -> instruction = initialize_instruction(NULL, 0, 0, 0, 0);
        instruction_or_data -> data = -1;
    }
    else if(instruction_or_data -> type_of_cache == 1){ // for L1 Data cache, assign instruction = -1 and data = 0
        instruction_or_data -> instruction = initialize_instruction(NULL, -1, -1, -1, -1);
        instruction_or_data -> data = 0;
    }
    else if(instruction_or_data -> type_of_cache == 2){ // for L2 unified cache, assign instruction = 0 and data = 0
        instruction_or_data -> instruction = initialize_instruction(NULL, 0, 0, 0, 0);
        instruction_or_data -> data = 0;
    }
    return instruction_or_data;
}