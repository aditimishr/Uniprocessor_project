#include <stdio.h>
#include "Cache/Cache.h"
#include "Main_Memory/Main_Memory.h"
#include "string.h"
#include "Processor/processor.h"

int main() {
    // first argument is for number of sets in the cache and second argument is for type of cache we want to build
    // 0 for L1 instruction
    // 1 for L1 data
    // 2 for L2 unified cache
    L1_Cache* L1_instruction_cache = initialize_L1_cache(64, 0);
    L1_Cache* L1_data_cache = initialize_L1_cache(64, 1);
    L2_Cache* L2_cache = initialize_L2_cache(512, 2);

    //printf("%s", L2_cache->pointer_to_set[1]->pointer_to_one_block[0]->each_int[6]->name_of_instruction_or_data);
//    int data = MainMemory->row[2]->each_column[1]->data;
//    printf("%d", data);
    run_CPU(L1_instruction_cache, L1_data_cache, L2_cache);
    //printf("Hello, World!\n");
    return 0;
}
