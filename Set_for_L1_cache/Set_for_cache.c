//
// Created by aditimishra on 5/26/2019.
//

#include "Set_for_cache.h"
#include "malloc.h"
#include "string.h"

L1_Set * initialize_L1_Set(Address* address, int set_associativity, int type_of_cache){
    L1_Set* set_desc = malloc(sizeof(L1_Set));
    set_desc -> set_associativity = set_associativity;
    set_desc -> address = address;
    set_desc -> type_of_cache = type_of_cache;
    int address_for_block = address -> decimal_address;
    for(int i = 0;i<set_associativity; i++){
        char bits_for_address_for_set[18];
        char* binary_number_for_set = decimal_to_binary_conversion(address_for_block);
        strcpy(bits_for_address_for_set, binary_number_for_set);
        Address* address_for_blocks_in_set = Assign_Address_to_L1(bits_for_address_for_set);
        address_for_blocks_in_set -> decimal_address = address_for_block;
        set_desc -> pointer_to_one_block[i] = initialize_L1_block(address_for_blocks_in_set, set_desc -> type_of_cache);
        address_for_block += 32;
    }
    return set_desc;
    //set_desc -> pointer_to_one_block = block;
}

L2_Set * initialize_L2_Set(Address* address, int set_associativity, int type_of_cache){
    L2_Set* set_desc = malloc(sizeof(L2_Set));
    set_desc -> set_associativity = set_associativity;
    set_desc -> address = address;
    set_desc -> type_of_cache = type_of_cache;
    int address_for_block = 0;//address -> decimal_address;
    for(int i = 0;i<set_associativity; i++){
        char bits_for_address_for_set[18];
        char* binary_number_for_set = decimal_to_binary_conversion(address_for_block);
        strcpy(bits_for_address_for_set, binary_number_for_set);
        Address* address_for_blocks_in_set = Assign_Address_to_L2(bits_for_address_for_set);
        address_for_blocks_in_set -> decimal_address = address_for_block;
        set_desc -> pointer_to_one_block[i] = initialize_L2_block(address_for_blocks_in_set, set_desc -> type_of_cache);
        //address_for_block += 32;
    }
    return set_desc;
    //set_desc -> pointer_to_one_block = block;
}