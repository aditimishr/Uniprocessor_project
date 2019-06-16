//
// Created by aditimishra on 6/15/2019.
//

#ifndef UNIPROCESSOR_ROWS_H
#define UNIPROCESSOR_ROWS_H


#include "../Instruction/Instruction.h"
#include "../In_a_row/In_a_row.h"
#include "../Cache/Cache.h"

// there are 8 columns in each row in main memory
typedef struct Rows{
    int number_of_columns_in_each_row;
    In_a_row* each_column[8];
} Rows;

Rows* initialize_main_memory_with_instruction_or_data(char instruction_or_data, int address, L1_Cache* L1_Cache, L2_Cache* L2_Cache);
void fill_instructions_in_L1_and_L2_cache(Instruction* instruction1, Address* address1, int column, L1_Cache* L1_Cache, L2_Cache* L2_Cache, int address,
                                          char* name_of_instruction);

#endif //UNIPROCESSOR_ROWS_H
