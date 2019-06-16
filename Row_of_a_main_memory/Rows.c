//
// Created by aditimishra on 5/31/2019.
//

#include "Rows.h"
#include "malloc.h"
#include "stdio.h"
#include "../Cache/Cache.h"
#include "string.h"

Rows* initialize_main_memory_with_instruction_or_data(char instruction_or_data, int address, L1_Cache* L1_Cache, L2_Cache* L2_Cache){
    Rows* rows = malloc(sizeof(Rows));
    rows -> number_of_columns_in_each_row = 8;
    int address_here = address;

    if (instruction_or_data == 'I') {

        // test case 1   Single read

        Instruction* instruction2 = initialize_instruction("I1", 10, 64, 4, -1);
        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[0] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 0, L1_Cache, L2_Cache, address_here, "I1");

        // test case 2   Single write

//        Instruction* instruction1 = initialize_instruction("I1", 20, 64, 4, 6);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");


        // test case 3   Single write followed by a read to the same address

//        Instruction* instruction1 = initialize_instruction("I1", 20, 64, 4, 6);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 10, 64, 4, -1);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");


        // test case 4      Back to back read to the same line

//        Instruction* instruction1 = initialize_instruction("I1", 10, 64, 4, -1);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 10, 68, 4, -1);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");
//
//        address_here = address_here + 4;
//        Instruction* instruction3 = initialize_instruction("I3", 10, 72, 4, -1);
//        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[2] = initialize_each_column_of_main_memory("I3", 0, instruction3, -1, address3);
//        fill_instructions_in_L1_and_L2_cache(instruction3, address3, 2, L1_Cache, L2_Cache, address_here, "I3");
//
//        address_here = address_here + 4;
//        Instruction* instruction4 = initialize_instruction("I4", 10, 76, 4, -1);
//        Address* address4 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[3] = initialize_each_column_of_main_memory("I4", 0, instruction4, -1, address4);
//        fill_instructions_in_L1_and_L2_cache(instruction4, address4, 3, L1_Cache, L2_Cache, address_here, "I4");

        // test case 5      Back to back write to the same line

//        Instruction* instruction1 = initialize_instruction("I1", 20, 64, 4, 4);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 20, 68, 4, 4);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I1", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");


        // test case 6    Back to back read to different line

//        Instruction* instruction1 = initialize_instruction("I1", 10, 64, 4, -1);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 10, 2112, 4, -0);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");
//
//        address_here = address_here + 4;
//        Instruction* instruction3 = initialize_instruction("I3", 10, 4160, 4, -0);
//        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[2] = initialize_each_column_of_main_memory("I3", 0, instruction3, -1, address3);
//        fill_instructions_in_L1_and_L2_cache(instruction3, address3, 2, L1_Cache, L2_Cache, address_here, "I3");


        // test case 7      Back to back write to different lines

//        Instruction* instruction1 = initialize_instruction("I1", 20, 64, 4, 4);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 20, 2112, 4, 5);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");
//
//        address_here = address_here + 4;
//        Instruction* instruction3 = initialize_instruction("I3", 20, 4160, 4, 6);
//        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[2] = initialize_each_column_of_main_memory("I3", 0, instruction3, -1, address3);
//        fill_instructions_in_L1_and_L2_cache(instruction3, address3, 2, L1_Cache, L2_Cache, address_here, "I3");


        // test case 8     Read from L1 victim cache

//        Instruction* instruction1 = initialize_instruction("I1", 10, 64, 4, -1);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 10, 2112, 4, -1);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");
//
//        address_here = address_here + 4;
//        Instruction* instruction3 = initialize_instruction("I3", 10, 4160, 4, -1);
//        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[2] = initialize_each_column_of_main_memory("I3", 0, instruction3, -1, address3);
//        fill_instructions_in_L1_and_L2_cache(instruction3, address3, 2, L1_Cache, L2_Cache, address_here, "I3");
//
//        address_here = address_here + 4;
//        Instruction* instruction4 = initialize_instruction("I4", 10, 6208, 4, -1);
//        Address* address4 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[3] = initialize_each_column_of_main_memory("I4", 0, instruction4, -1, address4);
//        fill_instructions_in_L1_and_L2_cache(instruction4, address4, 3, L1_Cache, L2_Cache, address_here, "I4");
//
//        address_here = address_here + 4;
//        Instruction* instruction5 = initialize_instruction("I5", 10, 8256, 4, -1);
//        Address* address5 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[4] = initialize_each_column_of_main_memory("I5", 0, instruction5, -1, address5);
//        fill_instructions_in_L1_and_L2_cache(instruction5, address5, 4, L1_Cache, L2_Cache, address_here, "I5");
//
//        address_here = address_here + 4;
//        Instruction* instruction6 = initialize_instruction("I6", 10, 10304, 4, -1);
//        Address* address6 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[5] = initialize_each_column_of_main_memory("I6", 0, instruction6, -1, address6);
//        fill_instructions_in_L1_and_L2_cache(instruction6, address6, 5, L1_Cache, L2_Cache, address_here, "I6");


        //  test case 9     Read from L1 Write Buffer

//        Instruction* instruction1 = initialize_instruction("I1", 20, 64, 4, 5);
//        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[0] = initialize_each_column_of_main_memory("I1", 0, instruction1, -1, address1);
//        fill_instructions_in_L1_and_L2_cache(instruction1, address1, 0, L1_Cache, L2_Cache, address_here, "I1");
//
//        address_here = address_here + 4;
//        Instruction* instruction2 = initialize_instruction("I2", 20, 2112, 4, 6);
//        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[1] = initialize_each_column_of_main_memory("I2", 0, instruction2, -1, address2);
//        fill_instructions_in_L1_and_L2_cache(instruction2, address2, 1, L1_Cache, L2_Cache, address_here, "I2");
//
//        address_here = address_here + 4;
//        Instruction* instruction3 = initialize_instruction("I3", 20, 4160, 4, 7);
//        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[2] = initialize_each_column_of_main_memory("I3", 0, instruction3, -1, address3);
//        fill_instructions_in_L1_and_L2_cache(instruction3, address3, 2, L1_Cache, L2_Cache, address_here, "I3");
//
//        address_here = address_here + 4;
//        Instruction* instruction4 = initialize_instruction("I4", 20, 6208, 4, 8);
//        Address* address4 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[3] = initialize_each_column_of_main_memory("I4", 0, instruction4, -1, address4);
//        fill_instructions_in_L1_and_L2_cache(instruction4, address4, 3, L1_Cache, L2_Cache, address_here, "I4");
//
//        address_here = address_here + 4;
//        Instruction* instruction5 = initialize_instruction("I5", 20, 8256, 4, 9);
//        Address* address5 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[4] = initialize_each_column_of_main_memory("I5", 0, instruction5, -1, address5);
//        fill_instructions_in_L1_and_L2_cache(instruction5, address5, 4, L1_Cache, L2_Cache, address_here, "I5");
//
//        Instruction* instruction6 = initialize_instruction("I6", 10, 64, 4, 5);
//        Address* address6 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
//        rows -> each_column[5] = initialize_each_column_of_main_memory("I5", 0, instruction6, -1, address6);
//        fill_instructions_in_L1_and_L2_cache(instruction6, address6, 5, L1_Cache, L2_Cache, address_here, "I6");

    }

    else if(instruction_or_data == 'D'){
        Instruction* instruction1 = NULL;
        Address* address1 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[0] = initialize_each_column_of_main_memory("D1", 0, instruction1, 20, address1);

        address_here = address_here + 4;
        Instruction* instruction2 = NULL;
        Address* address2 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[1] = initialize_each_column_of_main_memory("D2", 0, instruction2, 30, address2);

        address_here = address_here + 4;
        Instruction* instruction3 = NULL;
        Address* address3 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[2] = initialize_each_column_of_main_memory("D3", 0, instruction3, 40, address3);

        address_here = address_here + 4;
        Instruction* instruction4 = NULL;
        Address* address4 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[3] = initialize_each_column_of_main_memory("D4", 0, instruction4, 50, address4);

        address_here = address_here + 4;
        Instruction* instruction5 = NULL;
        Address* address5 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[4] = initialize_each_column_of_main_memory("D5", 0, instruction5, 60, address5);

        address_here = address_here + 4;
        Instruction* instruction6 = NULL;
        Address* address6 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[5] = initialize_each_column_of_main_memory("D6", 0, instruction6, 70, address6);

        address_here = address_here + 4;
        Instruction* instruction7 = NULL;
        Address* address7 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[6] = initialize_each_column_of_main_memory("D7", 0, instruction7, 80, address7);

        address_here = address_here + 4;
        Instruction* instruction8 = NULL;
        Address* address8 = Assign_Address_to_Memory(decimal_to_binary_conversion(address_here));
        rows -> each_column[7] = initialize_each_column_of_main_memory("D8", 0, instruction8, 90, address8);
    }
    return rows;
}

void fill_instructions_in_L1_and_L2_cache(Instruction* instruction1, Address* address1, int column, L1_Cache* L1_Cache, L2_Cache* L2_Cache, int address,
                                          char* name_of_instruction){
    int block_address = address/32;
    L1_Cache -> pointer_to_set[0] -> pointer_to_one_block[block_address]->each_int[column]->name_of_instruction_or_data = name_of_instruction;
    L1_Cache -> pointer_to_set[0] -> pointer_to_one_block[block_address]->each_int[column]->instruction = instruction1;
    L1_Cache -> pointer_to_set[0] -> pointer_to_one_block[block_address]->each_int[column]->address = address1;
    L1_Cache -> pointer_to_set[0] -> pointer_to_one_block[block_address]->each_int[column]->data = -1;
    L1_Cache -> pointer_to_set[0] -> pointer_to_one_block[block_address]->each_int[column]->type_of_cache = 0;

    L2_Cache -> pointer_to_sets[block_address] -> pointer_to_one_block[0]->each_int[column]->name_of_instruction_or_data = name_of_instruction;
    L2_Cache -> pointer_to_sets[block_address] -> pointer_to_one_block[0]->each_int[column]->instruction = instruction1;
    L2_Cache -> pointer_to_sets[block_address] -> pointer_to_one_block[0]->each_int[column]->address = address1;
    L2_Cache -> pointer_to_sets[block_address] -> pointer_to_one_block[0]->each_int[column]->data = -1;
    L2_Cache -> pointer_to_sets[block_address] -> pointer_to_one_block[0]->each_int[column]->type_of_cache = 0;
}