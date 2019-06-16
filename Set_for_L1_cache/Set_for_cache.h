//
// Created by aditimishra on 5/26/2019.
//

#ifndef UNIPROCESSOR_SET_FOR_CACHE_H
#define UNIPROCESSOR_SET_FOR_CACHE_H

#include "../Block/Block.h"
#include "../Address/Address.h"

typedef struct Set_for_L1_cache{
    //int number_of_blocks;     // It is the number of blocks in a set
    int set_associativity;
    Block *pointer_to_one_block[4];
    Address *address;
    int type_of_cache;
} L1_Set;

typedef struct Set_for_L2_cache{
    //int number_of_blocks;     // It is the number of blocks in a set
    int set_associativity;
    Block *pointer_to_one_block[4];
    Address *address;
    int type_of_cache;
} L2_Set;

L1_Set * initialize_L1_Set(Address* address, int set_associativity, int type_of_cache);
L2_Set * initialize_L2_Set(Address* address, int set_associativity, int type_of_cache);


#endif //UNIPROCESSOR_SET_FOR_CACHE_H
