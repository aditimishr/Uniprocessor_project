//
// Created by aditimishra on 5/30/2019.
//

#ifndef UNIPROCESSOR_MAIN_MEMORY_H
#define UNIPROCESSOR_MAIN_MEMORY_H

// Organisation of main memory = each row in the main memory contains 32 bytes
// So there will be 4096 rows in the main memory

#include "../Address/Address.h"
#include "../Instruction/Instruction.h"
#include "../Row_of_a_main_memory/Rows.h"
#include "../Cache/Cache.h"

typedef struct main_memory{
    int number_of_rows;
    Rows* row[4096];    // Each row has 8 columns
} Main_Memory;

Main_Memory* initialize_main_memory(int number_of_rows, L1_Cache* L1_Cache, L2_Cache* L2_Cache);



#endif //UNIPROCESSOR_MAIN_MEMORY_H
