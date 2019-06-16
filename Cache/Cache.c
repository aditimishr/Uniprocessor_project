//
// Created by aditimishra on 5/26/2019.
//

#include "malloc.h"
#include "Cache.h"
#include "stdio.h"
#include "string.h"

L1_Cache* initialize_L1_cache(int number_of_sets, int type_of_cache){
    L1_Cache* cache = malloc(sizeof(L1_Cache));
    cache -> number_of_sets = number_of_sets;
    cache -> type_of_cache = type_of_cache;

    // Now assign memory to each cache lines in a set

    int start_address = 0;
    for(int x = 0; x < cache -> number_of_sets; x++){
        char bits_for_address[18];         // total bits for address is 17 bits (size = 18 because it is char)
        // bits_for_address will store the binary data
        char* binary_number = decimal_to_binary_conversion(start_address);
        // assign this binary number to bits_for_address

        strcpy(bits_for_address, binary_number);
        Address* address = Assign_Address_to_L1(bits_for_address);
        address -> decimal_address = start_address;
        L1_Set* set1 = initialize_L1_Set(address, 4, cache -> type_of_cache);  // 4 is the set_associativity for the L1 cache
        cache -> pointer_to_set[x] = set1;
        start_address = start_address + 128;             //because we are assigning the address to each
    }
    return cache;
}

L2_Cache* initialize_L2_cache(int number_of_sets, int type_of_cache){
    L2_Cache* cache = malloc(sizeof(L2_Cache));
    cache -> number_of_sets = number_of_sets;
    cache -> type_of_cache = type_of_cache;

    // Now assign memory to each cache lines in a set

    int start_address = 0;
    for(int x = 0; x < cache -> number_of_sets; x++){
        char bits_for_address[18];         // total bits for address is 17 bits (size = 18 because it is char)
        // bits_for_address will store the binary data
        char* binary_number = decimal_to_binary_conversion(start_address);
        // assign this binary number to bits_for_address

        strcpy(bits_for_address, binary_number);
        Address* address = Assign_Address_to_L2(bits_for_address);
        address -> decimal_address = start_address;
        L2_Set* set2 = initialize_L2_Set(address, 1, cache -> type_of_cache);  // 1 is the set_associativity for the L2 cache
        cache -> pointer_to_sets[x] = set2;
        start_address = start_address + 32;             //because each set stores 32 ints in it
    }
    return cache;
}