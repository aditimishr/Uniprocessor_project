//
// Created by aditimishra on 5/26/2019.
//

#ifndef UNIPROCESSOR_BLOCK_H
#define UNIPROCESSOR_BLOCK_H


#include "../Address/Address.h"
#include "../In_a_cache_block/In_a_cache_block.h"

typedef struct Block_in_a_cache{
    int valid_bit;
    int dirty_bit;
    int number_of_ints_in_cache;
    int lru;
    In_a_block* each_int[8];
    int type_of_cache;
    Address *address;       //this is the data structure which contains all the information about the bits used in tag, index and offset
} Block;

Block* initialize_L1_block(Address* address, int type_of_cache);

Block* initialize_L2_block(Address* address, int type_of_cache);


#endif //UNIPROCESSOR_BLOCK_H
