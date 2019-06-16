cmake_minimum_required(VERSION 3.14)
project(Uniprocessor C)

set(CMAKE_C_STANDARD 99)

add_executable(Uniprocessor main.c Address/Address.c Address/Address.h Block/Block.c Block/Block.h Bus_L1_L2/Bus_L1_L2.c Bus_L1_L2/Bus_L1_L2.h Bus_L2_Memory/Bus_L2_Memory.c Bus_L2_Memory/Bus_L2_Memory.h Cache/Cache.c Cache/Cache.h Controller/Controller.c Controller/Controller.h Executing_instructions/Executing_instructions.c Executing_instructions/Executing_instructions.h In_a_cache_block/In_a_cache_block.c In_a_cache_block/In_a_cache_block.h In_a_row/In_a_row.c In_a_row/In_a_row.h Instruction/Instruction.c Instruction/Instruction.h Main_Memory/Main_Memory.c Main_Memory/Main_Memory.h Processor/processor.c Processor/processor.h Queue/Queue.c Queue/Queue.h Row_of_a_main_memory/Rows.c Row_of_a_main_memory/Rows.h Set_for_L1_cache/Set_for_cache.c Set_for_L1_cache/Set_for_cache.h Victim_Cache/Victim_Cache.c Victim_Cache/Victim_Cache.h Write_Buffer/Write_Buffer.c Write_Buffer/Write_Buffer.h)