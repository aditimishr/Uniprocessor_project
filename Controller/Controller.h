//
// Created by aditimishra on 6/2/2019.
//

#ifndef UNIPROCESSOR_CONTROLLER_H
#define UNIPROCESSOR_CONTROLLER_H

#include "../Bus_L1_L2/Bus_L1_L2.h"
#include "../Main_Memory/Main_Memory.h"
#include "../Bus_L2_Memory/Bus_L2_Memory.h"
#include "../Write_Buffer/Write_Buffer.h"
#include "../Victim_cache/Victim_Cache.h"
#include "../Executing_instructions/Executing_instructions.h"
#include "../Queue/Queue.h"

typedef struct L2_controller_1{
    Instruction* instruction;
    Queue* L2_read_port_queue;
    Queue* L2_write_port_queue;
    L2_Cache* l2Cache;
    Executing_instructions L2_executingInstructions;
    Bus_L1_and_L2* busL1AndL2;
    Bus_L1_and_L2* bus_L2_and_write_buffer;
    Main_Memory* mainMemory;
    Bus_L2_and_Memory* busL2AndMemory;
    Write_Buffer* writeBuffer;
} L2_controller;

typedef struct L1_controller_cont{
    Instruction* instruction_to_L1;
    Queue* L1_read_port_queue;
    Queue* L1_write_port_queue;
    L1_Cache* l1_instruction_Cache;
    L1_Cache* l1_data_Cache;
    Executing_instructions L1_executingInstructions;
    L2_controller* l2Controller;
    Bus_L1_and_L2* bus_for_L1_and_victim;
    Bus_L1_and_L2* bus_for_L1_and_write_buffer;
    Victim_Cache* victimCache;
    Write_Buffer* writeBuffer;
} L1_controller;

L1_controller* initialize_L1_controller(L1_Cache* l1_instruction_Cache, L1_Cache* l1_data_Cache, L2_Cache* l2Cache);

L1_controller* L1_read(L1_controller* l1Controller);

L1_controller* L1_write(L1_controller* l1_cont);

L2_controller* initialize_L2_controller(L1_Cache* l1_instruction_Cache, L2_Cache* l2Cache);

L2_controller* L2_read(L2_controller* l2Controller);

L2_controller* put_data_into_L2_block(L2_controller* l2Controller, In_a_row* column, Address* address_for_memory, int set_six_bit);

L1_Set* set_lru_bit(L1_Set *pCache, int block_number, char* charac);

Victim_Cache *set_lru_bit_for_victim_cache(Victim_Cache *pCache1, int position, char* charac);

Write_Buffer *set_lru_bit_for_write_buffer(Write_Buffer *pBuffer, int buffer, char* strin);

L1_controller *update_L2_from_write_buffer(L1_controller *l1Controller);

L2_controller *update_memory_from_L2_write_buffer(L2_controller *pController1);


#endif //UNIPROCESSOR_CONTROLLER_H
