//
// Created by aditimishra on 5/26/2019.
//

#include "Block.h"
#include "malloc.h"
#include "string.h"

Block* initialize_L1_block(Address* address, int type_of_cache){
    Block* block = malloc(sizeof(Block));
    block -> address = address;
    block -> dirty_bit = 0;
    block -> valid_bit = 0;
    block -> type_of_cache = type_of_cache;
    block -> lru = 0;
    block -> number_of_ints_in_cache = 8;
    int address_for_each_instruction_or_data = address -> decimal_address;
    for(int i = 0; i < block -> number_of_ints_in_cache; i++){
        char bits_for_address_for_block[18];
        char* binary_number_for_block = decimal_to_binary_conversion(address_for_each_instruction_or_data);
        strcpy(bits_for_address_for_block, binary_number_for_block);
        Address* address_for = Assign_Address_to_L1(bits_for_address_for_block);
        address_for -> decimal_address = address_for_each_instruction_or_data;
        block -> each_int[i] = inititialize_with_instruction_or_data_or_both(address_for, block -> type_of_cache);
        address_for_each_instruction_or_data += 4;
    }
    return block;
    //block -> each_int = inABlock;
}

Block* initialize_L2_block(Address* address, int type_of_cache){
    Block* block = malloc(sizeof(Block));
    block -> address = address;
    block -> dirty_bit = 0;
    block -> valid_bit = 0;
    block -> type_of_cache = type_of_cache;
    block -> lru = 0;
    block -> number_of_ints_in_cache = 8;
    int address_for_each_instruction_or_data = address -> decimal_address;
    for(int i = 0; i < block -> number_of_ints_in_cache; i++){
        char bits_for_address_for_block[18];
        char* binary_number_for_block = decimal_to_binary_conversion(address_for_each_instruction_or_data);
        strcpy(bits_for_address_for_block, binary_number_for_block);
        Address* address_for = Assign_Address_to_L2(bits_for_address_for_block);
        address_for -> decimal_address = address_for_each_instruction_or_data;
        block -> each_int[i] = inititialize_with_instruction_or_data_or_both(address_for, block -> type_of_cache);
        address_for_each_instruction_or_data += 4;
    }
    return block;
    //block -> each_int = inABlock;
}
