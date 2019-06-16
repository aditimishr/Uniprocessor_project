//
// Created by aditimishra on 5/30/2019.
//

#include "Main_Memory.h"
#include "malloc.h"
#include "stdio.h"

Main_Memory* initialize_main_memory(int number_of_rows, L1_Cache* L1_Cache, L2_Cache* L2_Cache){
    Main_Memory* mainMemory = malloc(sizeof(Main_Memory));
    L1_Cache->pointer_to_set[0]->pointer_to_one_block[0]->valid_bit = 1;
    L1_Cache->pointer_to_set[0]->pointer_to_one_block[0]->dirty_bit = 0;
    L1_Cache->pointer_to_set[0]->pointer_to_one_block[1]->valid_bit = 1;
    L1_Cache->pointer_to_set[0]->pointer_to_one_block[1]->dirty_bit = 0;

    L2_Cache->pointer_to_sets[0]->pointer_to_one_block[0]->valid_bit = 1;
    L2_Cache->pointer_to_sets[0]->pointer_to_one_block[0]->dirty_bit = 0;
    L2_Cache->pointer_to_sets[1]->pointer_to_one_block[0]->valid_bit = 1;
    L2_Cache->pointer_to_sets[1]->pointer_to_one_block[0]->dirty_bit = 0;

    mainMemory -> row[0] = initialize_main_memory_with_instruction_or_data('I', 0, L1_Cache, L2_Cache);  // 0 is the address in integer
    mainMemory -> row[1] = initialize_main_memory_with_instruction_or_data('I', 32, L1_Cache, L2_Cache);
    mainMemory -> row[2] = initialize_main_memory_with_instruction_or_data('D', 64, NULL, NULL);
    mainMemory -> row[66] = initialize_main_memory_with_instruction_or_data('D', 2112, NULL, NULL);
    mainMemory -> row[130] = initialize_main_memory_with_instruction_or_data('D', 4160, NULL, NULL);
    mainMemory -> row[194] = initialize_main_memory_with_instruction_or_data('D', 6208, NULL, NULL);
    mainMemory -> row[258] = initialize_main_memory_with_instruction_or_data('D', 8256, NULL, NULL);
    mainMemory -> row[322] = initialize_main_memory_with_instruction_or_data('D', 10304, NULL, NULL);
    mainMemory -> row[386] = initialize_main_memory_with_instruction_or_data('D', 12352, NULL, NULL);
    mainMemory -> row[450] = initialize_main_memory_with_instruction_or_data('D', 14400, NULL, NULL);
    return mainMemory;
}