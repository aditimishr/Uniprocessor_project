//
// Created by aditimishra on 5/26/2019.
//

#ifndef UNIPROCESSOR_CACHE_H
#define UNIPROCESSOR_CACHE_H

#include "../Set_for_L1_cache/Set_for_cache.h"

typedef struct L1_Cache_struct{
    int number_of_sets;     // this is the number of sets in the cache
    // {for L1, it is 2^6 because it is set associative
    // and for L2, it is 2^9 because it is direct mapped}
    L1_Set *pointer_to_set[64];
    int type_of_cache;
} L1_Cache;

typedef struct L2_Cache_struct{
    int number_of_sets;     // this is the number of sets in the cache
    // {for L1, it is 2^6 because it is set associative
    // and for L2, it is 2^9 because it is direct mapped}
    L2_Set *pointer_to_sets[512];
    int type_of_cache;
} L2_Cache;

L1_Cache* initialize_L1_cache(int number_of_sets, int type_of_cache);
L2_Cache* initialize_L2_cache(int number_of_sets, int type_of_cache);




#endif //UNIPROCESSOR_CACHE_H
