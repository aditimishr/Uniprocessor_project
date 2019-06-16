//
// Created by aditimishra on 5/30/2019.
//

#ifndef UNIPROCESSOR_IN_A_CACHE_BLOCK_H
#define UNIPROCESSOR_IN_A_CACHE_BLOCK_H

#include "../Address/Address.h"
#include "../Instruction/Instruction.h"

// for L1, two instances (one for instruction and one for data) -
// for instruction cache initialize instruction with 0 and data with -1 and vice-versa
// for L2 cache, we can initialize both data and instruction with 0 and can check whether
// the given location has data or instruction with the if-else loop.
// And if both are 0, the space in the block is empty.

typedef struct In_a_cache_block{
    char *name_of_instruction_or_data;
    Address *address;           // this is the address at which this instruction or data resides
    Instruction* instruction;
    int data;
    int type_of_cache;
} In_a_block;

In_a_block* inititialize_with_instruction_or_data_or_both(Address* address, int type_of_cache);



#endif //UNIPROCESSOR_IN_A_CACHE_BLOCK_H
