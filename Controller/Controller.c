//
// Created by aditimishra on 6/2/2019.
//

#include "string.h"
#include "stdio.h"
#include "malloc.h"
#include "Controller.h"
#include "../Bus_L1_L2/Bus_L1_L2.h"


L2_controller *update_memory_from_L2_write_buffer(L2_controller *pController1);

L1_controller* initialize_L1_controller(L1_Cache* l1_instruction_Cache, L1_Cache* l1_data_Cache, L2_Cache* l2Cache){
    L1_controller* l1Controller = malloc(sizeof(L1_controller));
    l1Controller -> instruction_to_L1 = NULL;
    l1Controller -> L1_read_port_queue = createQueue(1000);
    l1Controller -> L1_write_port_queue = createQueue(1000);
    l1Controller -> l1_instruction_Cache = l1_instruction_Cache;
    l1Controller -> l1_data_Cache = l1_data_Cache;
    l1Controller -> L1_executingInstructions = iniatialize_Executing_instructions();
    l1Controller -> l2Controller = initialize_L2_controller(l1_instruction_Cache, l2Cache);
    l1Controller -> bus_for_L1_and_victim = initialize_bus_between_L1_L2();
    l1Controller -> bus_for_L1_and_write_buffer = initialize_bus_between_L1_L2();
    l1Controller -> writeBuffer = initialize_write_buffer();
    l1Controller -> victimCache = initialize_victim_cache();
}

L2_controller* initialize_L2_controller(L1_Cache* l1_instruction_cache, L2_Cache* l2Cache){
    L2_controller* l2Controller = malloc(sizeof(L2_controller));
    l2Controller -> instruction = NULL;
    l2Controller -> L2_read_port_queue = createQueue(1000);
    l2Controller -> L2_write_port_queue = createQueue(1000);
    l2Controller -> l2Cache = l2Cache;
    l2Controller -> busL1AndL2 = initialize_bus_between_L1_L2();
    l2Controller -> mainMemory = initialize_main_memory(4096, l1_instruction_cache, l2Controller -> l2Cache);
    l2Controller -> busL2AndMemory = initialize_bus_between_L2_Memory();
    l2Controller -> bus_L2_and_write_buffer = initialize_bus_between_L1_L2();
    l2Controller -> writeBuffer = initialize_write_buffer();
}

L1_controller* L1_read(L1_controller* l1Controller){
    char* state = l1Controller -> L1_executingInstructions.state;
    char * address_char = decimal_to_binary_conversion(l1Controller -> L1_executingInstructions.instruction->address);
    Address* address_coming = Assign_Address_to_L1(address_char);
    int tag_to_find = address_coming->tag;
    int set_six_bit = address_coming -> set_or_index;
    int offet = address_coming -> offset;

    if(strcmp(state, "P to L1C") == 0){
        int found = 0;

        for (int j = 0; j< 4 && found == 0; j++){  // find if the data is there in data cache
            if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->address->tag == tag_to_find && l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->valid_bit == 1){
                strcpy(l1Controller -> L1_executingInstructions.state, "L1C to L1D : Hit");
                found += 1;
                break;
            }
        }
        if(found == 0) {           // if it is a miss
            // check in victim cache and write buffers
            int found_in_victim_cache = 0;
            int found_in_write_buffer = 0;
            for(int k = 0; k < 2 ; k++){
                if(l1Controller -> victimCache -> block[k]!= NULL && l1Controller -> victimCache ->block[k]->valid_bit == 1 && l1Controller -> victimCache ->block[k]->address -> tag == tag_to_find){
                    found_in_victim_cache += 1;
                    break;
                }
            }

            for(int k = 0; k < 2 ; k++){
                if(l1Controller -> writeBuffer -> block[k]!= NULL && l1Controller -> writeBuffer ->block[k]->valid_bit == 1 && l1Controller -> writeBuffer ->block[k]->address -> tag == tag_to_find){
                    found_in_write_buffer += 1;
                    break;
                }
            }

            // same way for write buffer

            if (found_in_victim_cache > 0){
                strcpy(l1Controller -> L1_executingInstructions.state, "Victim to L1C");
            }
            else if(found_in_write_buffer > 0 ){
                strcpy(l1Controller -> L1_executingInstructions.state, "Write Buffer to LIC");
            }
            else{      // its a miss in victim cache and in write buffer
                strcpy(l1Controller -> L1_executingInstructions.state , "L1C to L2C");
            }
        }
    }

    else if(strcmp(state, "L1C to L1D : Hit") == 0){
        strcpy(l1Controller -> L1_executingInstructions.state , "L1D to L1C");
    }
    else if(strcmp(state, "L1D to L1C") == 0){
        strcpy(l1Controller -> L1_executingInstructions.state, "L1C to P");
    }
    else if(strcmp(state, "L1C to L2C") == 0){
//            if(executingInstructions[i].instruction->type_of_instruction == 10){
        // It will perform the read operation and try to bring back the block in L1
        enqueue(l1Controller -> L1_read_port_queue, l1Controller -> L1_executingInstructions.instruction);
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state, "L1C to L1D and P") == 0) {
        int done = 0;
        for (int f = 0; f < 4; f++) {  // if the address is found
            if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[f]->valid_bit == 1 &&
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[f]->address->tag ==
                tag_to_find &&
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[f]->address->set_or_index ==
                set_six_bit) {
                strcpy(l1Controller->L1_executingInstructions.state, "Done");
                done += 1;
            }
        }
        if (done == 0) {            // address not found
            int empty_slot = -1;
            int position = -1;
            for (int j = 0;
                 j < 4 && empty_slot == -1; j++) { //finding the empty slot or finding the position of lru = 4 to be replaced
                if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->valid_bit == 0) {
                    empty_slot = j;
                } else if (
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->valid_bit !=
                        0 &&
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->lru == 4) {
                    position = j;
                }
            }
            if (empty_slot != -1) {
                for (int b = 0; b < 8; b++) {
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->each_int[b] =
                            l1Controller->l2Controller->busL1AndL2->block->each_int[b];
                }
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->valid_bit = 1;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->dirty_bit = 0;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->address = address_coming;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], empty_slot, "empty");

            } else if (empty_slot == -1) {
                // slot is full, place the block in victim cache or write buffer
                if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit ==
                    0) {
                    int empty_slot_in_victim = -1;
                    for (int v = 0; v < 2; v++) {
                        if (l1Controller->victimCache->block[v]->valid_bit != 1) {
                            empty_slot_in_victim = v;
                            break;
                        }
                    }
                    int position_in_victim = -1;
                    if (empty_slot_in_victim == -1) {  // check which block to replace
                        for (int v = 0; v < 2; v++) {
                            if (l1Controller->victimCache->block[v]->lru == 2) {
                                position_in_victim = v;
                            }
                        }
                    }
                    if (empty_slot_in_victim != -1) {    // victim slot is empty
                        for (int r = 0; r < 8; r++) {
                            l1Controller->victimCache->block[empty_slot_in_victim]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                        }
                        l1Controller->victimCache->block[empty_slot_in_victim]->valid_bit = 1;
                        l1Controller->victimCache->block[empty_slot_in_victim]->dirty_bit = 0;
                        l1Controller->victimCache->block[empty_slot_in_victim]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                        l1Controller->victimCache = set_lru_bit_for_victim_cache(l1Controller->victimCache,
                                                                                 empty_slot_in_victim, "empty");

                    } else {                       // victim slot is not empty
                        for (int r = 0; r < 8; r++) {
                            l1Controller->victimCache->block[position_in_victim]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                        }
                        l1Controller->victimCache->block[position_in_victim]->valid_bit = 1;
                        l1Controller->victimCache->block[position_in_victim]->dirty_bit = 0;
                        l1Controller->victimCache->block[position_in_victim]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                        l1Controller->victimCache = set_lru_bit_for_victim_cache(l1Controller->victimCache,
                                                                                 position_in_victim, "empty");
                    }
                    //int lru_bit = l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[position]->lru;
                    for (int b = 0; b < 8; b++) {
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] =
                                l1Controller->l2Controller->busL1AndL2->block->each_int[b];
                    }
                    //l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address-> set_or_index]->pointer_to_one_block[position]->lru = lru_bit;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], position, "");
                }
                else{
                    int empty_slot_in_write_buffer = -1;
                    for (int v = 0; v < 2; v++) {
                        if (l1Controller->writeBuffer->block[v]->valid_bit != 1) {
                            empty_slot_in_write_buffer = v;
                            break;
                        }
                    }
                    int position_in_write = -1;
                    if (empty_slot_in_write_buffer == -1) {  // check which block to replace
                        for (int v = 0; v < 2; v++) {
                            if (l1Controller->writeBuffer->block[v]->lru == 2) {
                                position_in_write = v;
                            }
                        }
                    }
                    if (empty_slot_in_write_buffer != -1) {    // write buffer is empty
                        for (int r = 0; r < 8; r++) {
                            l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                        }
                        l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->valid_bit = 1;
                        l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->dirty_bit = 1;
                        l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                        l1Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                                 empty_slot_in_write_buffer, "empty");

                    } else {                       // write buffer is not empty
                        // incase no slot is empty in write buffer, so place the lru = 2 block in bus of write buffer
                        // bus_for_L1_and_write_buffer is for lru = 2 block to place it in L2
                        if(l1Controller -> bus_for_L1_and_write_buffer -> is_empty == 0){
                            for(int w = 0; w < 8; w++){
                                l1Controller -> bus_for_L1_and_write_buffer->block->each_int[w] = l1Controller->writeBuffer->block[position_in_write]->each_int[w];
                            }
                            l1Controller -> bus_for_L1_and_write_buffer->is_empty = 1;
                            l1Controller->bus_for_L1_and_write_buffer->block->address = l1Controller->writeBuffer->block[position_in_write]->address;
                            l1Controller->bus_for_L1_and_write_buffer->block->valid_bit = 1;
                            l1Controller ->bus_for_L1_and_write_buffer-> block->dirty_bit = 1;
                            l1Controller = update_L2_from_write_buffer(l1Controller);
                        }

                        for (int r = 0; r < 8; r++) {
                            l1Controller->writeBuffer->block[position_in_write]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                        }
                        l1Controller->writeBuffer->block[position_in_write]->valid_bit = 1;
                        l1Controller->writeBuffer->block[position_in_write]->dirty_bit = 1;
                        l1Controller->writeBuffer->block[position_in_write]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                        l1Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                                 position_in_write, "");
                    }
                    //int lru_bit = l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[position]->lru;
                    for (int b = 0; b < 8; b++) {
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] =
                                l1Controller->l2Controller->busL1AndL2->block->each_int[b];
                    }
                    //l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address-> set_or_index]->pointer_to_one_block[position]->lru = lru_bit;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], position, "");

                }
            }
            l1Controller->l2Controller->busL1AndL2 = initialize_bus_between_L1_L2();
            strcpy(l1Controller->L1_executingInstructions.state, "Done");
        }
    }
    else if(strcmp(state, "L2C to Memory") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if(strcmp(state, "Memory to L2C") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state, "L2C to L1C") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state,"Memory to L2C : Block") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if(strcmp(state, "Victim to L1C") == 0){
        int found_in_victim_cache = 0;
        int position_in_victim = -1;
        for(int k = 0; k < 2 ; k++){  // find the position where the tag is found
            if(l1Controller -> victimCache -> block[k]!= NULL && l1Controller -> victimCache ->block[k]->valid_bit == 1 && l1Controller -> victimCache ->block[k]->address -> set_or_index == set_six_bit && l1Controller -> victimCache ->block[k]->address -> tag == tag_to_find){
                found_in_victim_cache += 1;
                position_in_victim = k;
                break;
            }
        }
        int replace = 0;
        int position = -1;
        if(found_in_victim_cache > 0){
            // first put the block with lru = 4 from L1 data in the bus and then replace that L1data place with the tag from victim cache
            for(int l = 0; l < 4 ; l++){
                if(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->lru == 4){
                    if(l1Controller -> bus_for_L1_and_victim -> is_empty == 0){
                        replace = 1;
                        position = l;
                        for(int b = 0; b < 8; b++){
                            l1Controller -> bus_for_L1_and_victim -> block->each_int[b] = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->each_int[b];
                        }
                        //l1Controller -> bus_for_L1_and_victim -> block->lru = 0;
                        l1Controller -> bus_for_L1_and_victim ->address = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->address;
                        l1Controller -> bus_for_L1_and_victim -> is_empty = 1;
                    }
                }
            }
            if(replace == 1){
                for(int b = 0; b < 8; b++){
                    l1Controller -> l1_data_Cache ->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] = l1Controller -> victimCache -> block[position_in_victim]->each_int[b];
                }
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit] = set_lru_bit(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit], position, "");
                for(int b = 0; b < 8; b++){
                    l1Controller -> victimCache->block[position_in_victim]->each_int[b] = l1Controller -> bus_for_L1_and_victim -> block->each_int[b];
                }
                l1Controller -> victimCache -> block[position_in_victim]->dirty_bit = 0;
                l1Controller -> victimCache -> block[position_in_victim]->valid_bit = 1;
                l1Controller -> victimCache -> block[position_in_victim]->address = l1Controller -> bus_for_L1_and_victim -> address;
                l1Controller -> victimCache = set_lru_bit_for_victim_cache(l1Controller -> victimCache, position_in_victim, "");
                l1Controller -> bus_for_L1_and_victim = initialize_bus_between_L1_L2();
                strcpy(l1Controller -> L1_executingInstructions.state, "L1C to L1D and P");
            }
        }
    }
    else if(strcmp(state, "Write Buffer to LIC") == 0){
        int found_in_write_buffer = 0;
        int position_in_buffer = -1;
        for(int k = 0; k < 2 ; k++){  // find the position where the tag is found
            if(l1Controller -> writeBuffer -> block[k]!= NULL && l1Controller -> writeBuffer ->block[k]->valid_bit == 1 && l1Controller -> writeBuffer ->block[k]->address -> set_or_index == set_six_bit && l1Controller -> writeBuffer ->block[k]->address -> tag == tag_to_find){
                found_in_write_buffer += 1;
                position_in_buffer = k;
                break;
            }
        }
        int replace = 0;
        int position = -1;
        if(found_in_write_buffer > 0){
            // first put the block with lru = 4 from L1 data in the bus and then replace that L1data place with the tag from write buffer
            for(int l = 0; l < 4 ; l++){
                if(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->lru == 4){
                    if(l1Controller -> bus_for_L1_and_write_buffer -> is_empty == 0){
                        replace = 1;
                        position = l;
                        for(int b = 0; b < 8; b++){
                            l1Controller -> bus_for_L1_and_write_buffer -> block->each_int[b] = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->each_int[b];
                        }
                        l1Controller -> bus_for_L1_and_write_buffer ->address = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->address;
                        l1Controller -> bus_for_L1_and_write_buffer -> is_empty = 1;
                    }
                }
            }
            if(replace == 1){
                for(int b = 0; b < 8; b++){
                    l1Controller -> l1_data_Cache ->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] = l1Controller -> writeBuffer -> block[position_in_buffer]->each_int[b];
                }
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 1;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit] = set_lru_bit(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit], position, "");
                for(int b = 0; b < 8; b++){
                    l1Controller -> writeBuffer->block[position_in_buffer]->each_int[b] = l1Controller -> bus_for_L1_and_write_buffer -> block->each_int[b];
                }
                l1Controller -> writeBuffer -> block[position_in_buffer]->dirty_bit = 1;
                l1Controller -> writeBuffer -> block[position_in_buffer]->valid_bit = 1;
                l1Controller -> writeBuffer -> block[position_in_buffer]->address = l1Controller -> bus_for_L1_and_write_buffer -> address;
                l1Controller -> writeBuffer = set_lru_bit_for_write_buffer(l1Controller -> writeBuffer, position, "");
                l1Controller -> bus_for_L1_and_write_buffer = initialize_bus_between_L1_L2();
                strcpy(l1Controller -> L1_executingInstructions.state, "L1C to L1D and P");
            }
        }

    }
    return l1Controller;
}

L1_controller *update_L2_from_write_buffer(L1_controller *l1Controller) {
    Address* address = l1Controller -> bus_for_L1_and_write_buffer->address;
    Address* parsed_address = Assign_Address_to_L2(address -> address_character);
    if(l1Controller -> l2Controller->l2Cache->pointer_to_sets[parsed_address->set_or_index]->pointer_to_one_block[0]->valid_bit == 1 &&
       l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address-> set_or_index]->pointer_to_one_block[0]->dirty_bit == 1){

        // place L2 block in L2 write buffer
        int empty_slot_in_write_buffer = -1;
        for (int v = 0; v < 2; v++) {
            if (l1Controller->l2Controller->writeBuffer->block[v]->valid_bit != 1) {
                empty_slot_in_write_buffer = v;
                break;
            }
        }

        int position_in_write = -1;
        if (empty_slot_in_write_buffer == -1) {  // check which block to replace
            for (int v = 0; v < 2; v++) {
                if (l1Controller->l2Controller->writeBuffer->block[v]->lru == 2) {
                    position_in_write = v;
                }
            }
        }

        if(empty_slot_in_write_buffer != -1){  // in case any of the slot is empty in L2 write buffer
            for(int i= 0;i< 8;i++){
                l1Controller-> l2Controller->writeBuffer->block[empty_slot_in_write_buffer]->each_int[i] = l1Controller -> l2Controller ->
                        l2Cache -> pointer_to_sets[parsed_address->set_or_index]->pointer_to_one_block[0]->each_int[i];
            }
            l1Controller-> l2Controller->writeBuffer->block[empty_slot_in_write_buffer]->dirty_bit = 1;
            l1Controller-> l2Controller->writeBuffer->block[empty_slot_in_write_buffer]->valid_bit = 1;
            l1Controller-> l2Controller->writeBuffer->block[empty_slot_in_write_buffer]->address = l1Controller -> l2Controller ->
                    l2Cache -> pointer_to_sets[parsed_address->set_or_index]->pointer_to_one_block[0]->address;
            l1Controller-> l2Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                                    empty_slot_in_write_buffer, "empty");
        }
        else {                       // write buffer is not empty
            // incase no slot is empty in write buffer, so place the lru = 2 block in bus of write buffer
            // bus_L2_and_write will work for memory
            if(l1Controller -> l2Controller -> bus_L2_and_write_buffer -> is_empty == 0){
                for(int w = 0; w < 8; w++){
                    l1Controller -> l2Controller -> bus_L2_and_write_buffer-> block->each_int[w] = l1Controller->l2Controller->writeBuffer->block[position_in_write]->each_int[w];
                }
                l1Controller ->l2Controller ->  bus_L2_and_write_buffer->is_empty = 1;
                l1Controller->l2Controller->bus_L2_and_write_buffer->block->address = l1Controller->l2Controller->writeBuffer->block[position_in_write]->address;
                l1Controller->l2Controller->bus_L2_and_write_buffer->block->valid_bit = 1;
                l1Controller ->l2Controller->bus_L2_and_write_buffer-> block->dirty_bit = 1;
                l1Controller-> l2Controller = update_memory_from_L2_write_buffer(l1Controller-> l2Controller);
            }

            for (int r = 0; r < 8; r++) {
                l1Controller-> l2Controller->writeBuffer->block[position_in_write]->each_int[r] = l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address-> set_or_index]->pointer_to_one_block[0]->each_int[r];
            }
            l1Controller->l2Controller->writeBuffer->block[position_in_write]->valid_bit = 1;
            l1Controller->l2Controller->writeBuffer->block[position_in_write]->dirty_bit = 1;
            l1Controller->l2Controller->writeBuffer->block[position_in_write]->address = l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address->set_or_index]->pointer_to_one_block[0]->address;
            l1Controller->l2Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                                   position_in_write, "");
        }

    }

    // now place the block in L2
    for(int i = 0; i < 8;i++){
        l1Controller -> l2Controller->l2Cache->pointer_to_sets[parsed_address->set_or_index]->pointer_to_one_block[0]->each_int[i] =
                l1Controller -> bus_for_L1_and_write_buffer ->block->each_int[i];
    }
    l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address-> set_or_index]->pointer_to_one_block[0]->address = parsed_address;
    l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address-> set_or_index]->pointer_to_one_block[0]->dirty_bit = 1;
    l1Controller->l2Controller->l2Cache->pointer_to_sets[parsed_address-> set_or_index]->pointer_to_one_block[0]->valid_bit = 1;
    l1Controller -> bus_for_L1_and_write_buffer = initialize_bus_between_L1_L2();
    return l1Controller;
}

L2_controller *update_memory_from_L2_write_buffer(L2_controller *l2Controller) {
    Address* address = l2Controller -> bus_L2_and_write_buffer-> address;
    Address* address_parsed = Assign_Address_to_Memory(address -> address_character);
    for(int i = 0;i<8;i++){
        l2Controller -> mainMemory ->row[address_parsed->set_or_index]->each_column[i]->address =
                l2Controller -> bus_L2_and_write_buffer->block->each_int[i]->address;
        l2Controller -> mainMemory ->row[address_parsed->set_or_index]->each_column[i]->instruction =
                l2Controller -> bus_L2_and_write_buffer->block->each_int[i]->instruction;
        l2Controller -> mainMemory ->row[address_parsed->set_or_index]->each_column[i]->data =
                l2Controller -> bus_L2_and_write_buffer->block->each_int[i]->data;
        l2Controller -> mainMemory ->row[address_parsed->set_or_index]->each_column[i]->name_of_instruction_or_data =
                l2Controller -> bus_L2_and_write_buffer->block->each_int[i]->name_of_instruction_or_data;
    }
    l2Controller -> bus_L2_and_write_buffer = initialize_bus_between_L1_L2();
    return l2Controller;
}

Write_Buffer *set_lru_bit_for_write_buffer(Write_Buffer *pBuffer, int buffer, char* string) {
    if(strcmp(string, "empty")==0){
        for(int i = 0; i < 2; i++){
            if(pBuffer -> block[i]!= NULL && pBuffer -> block[i] -> valid_bit == 1){
                pBuffer->block[i]->lru += 1;
            }
        }
        pBuffer -> block[buffer]->lru = 1;
        return pBuffer;
    }
    else{
        int lru = pBuffer -> block[buffer]->lru;
        for(int i = 0; i < lru; i++){
            if(pBuffer -> block[i]!= NULL && pBuffer -> block[i] -> valid_bit == 1){
                pBuffer->block[i]->lru += 1;
            }
        }
        pBuffer -> block[buffer]->lru = 1;
        return pBuffer;
    }
}


Victim_Cache *set_lru_bit_for_victim_cache(Victim_Cache *pCache1, int position, char* character_or) {
    if(strcmp(character_or, "empty")==0){
        for(int i = 0; i < 2; i++){
            if(pCache1 -> block[i]!= NULL && pCache1 -> block[i] -> valid_bit == 1){
                pCache1->block[i]->lru += 1;
            }
        }
        pCache1 -> block[position]->lru = 1;
        return pCache1;
    }
    else{
        int lru = pCache1 -> block[position]->lru;
        for(int i = 0; i < lru; i++){
            if(pCache1 -> block[i]!= NULL && pCache1 -> block[i] -> valid_bit == 1){
                pCache1->block[i]->lru += 1;
            }
        }
        pCache1 -> block[position]->lru = 1;
        return pCache1;
    }
}

L1_Set* set_lru_bit(L1_Set *pCache, int block_number, char* character_or) {
    if(strcmp(character_or, "empty") == 0){
        for(int i = 0; i < 4; i++){
            if(pCache -> pointer_to_one_block[i] != NULL && pCache -> pointer_to_one_block[i]->valid_bit == 1){
                pCache->pointer_to_one_block[i]->lru += 1;
            }
        }
        pCache -> pointer_to_one_block[block_number]->lru = 1;
        return pCache;
    }
    else{
        int lru = pCache -> pointer_to_one_block[block_number]->lru;
        for(int i = 0; i < lru; i++){
            if(pCache -> pointer_to_one_block[i] != NULL && pCache -> pointer_to_one_block[i]->valid_bit == 1){
                pCache->pointer_to_one_block[i]->lru += 1;
            }
        }
        pCache -> pointer_to_one_block[block_number]->lru = 1;
        return pCache;
    }
}

L2_controller* L2_read(L2_controller* l2Controller) {
    char *state = l2Controller -> L2_executingInstructions.state;
    char *address_char = decimal_to_binary_conversion(l2Controller -> L2_executingInstructions.instruction->address);
    Address *address_coming = Assign_Address_to_L2(address_char);
    int tag_to_find = address_coming->tag;
    int set_six_bit = address_coming->set_or_index;
    int offet = address_coming->offset;
    if (strcmp(state, "L1C to L2C") == 0) {
        if (l2Controller->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->valid_bit ==
            1) { // check for the address is valid or not
            if (l2Controller->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->address->tag ==
                tag_to_find) {   // if valid, check for the tag
                strcpy(l2Controller -> L2_executingInstructions.state, "L2C to L1C");
            } else {  // if the tag is not find
                strcpy(l2Controller -> L2_executingInstructions.state, "L2C to Memory");
            }
        } else if (l2Controller->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->valid_bit ==
                   0) {  // not valid
            strcpy(l2Controller -> L2_executingInstructions.state, "L2C to Memory");
        }
    } else if (strcmp(l2Controller -> L2_executingInstructions.state, "L2C to L1C") ==
               0) {  // place the data from L2_cache to L1_Data_cache
        if(l2Controller -> busL1AndL2 -> is_empty == 0){
            Block* block;
            l2Controller -> busL1AndL2 -> block = block;
            for(int b = 0; b < 8; b++){
                l2Controller -> busL1AndL2 -> block->each_int[b] = l2Controller->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[b];
            }
            l2Controller -> busL1AndL2 -> address = address_coming;
            l2Controller -> busL1AndL2 -> is_empty = 1;
            if(l2Controller -> L2_executingInstructions.instruction->type_of_instruction == 10){
                strcpy(l2Controller -> L2_executingInstructions.state, "L1C to L1D and P");
            }
            else{
                strcpy(l2Controller -> L2_executingInstructions.state, "Update L1C");
            }
        }
        else{
            strcpy(l2Controller -> L2_executingInstructions.state, "L2C to L1C");
        }
    }
    else if (strcmp(l2Controller -> L2_executingInstructions.state, "L2C to Memory") == 0){
        if(l2Controller -> l2Cache ->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->valid_bit == 1){  // check if the address in L2 is valid and dirty
            if(l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->dirty_bit == 1){
                Address* address_for_mem = Assign_Address_to_Memory(address_char);
                for(int m = 0; m < 8; m++){
                    l2Controller -> mainMemory ->row[address_for_mem->set_or_index]->each_column[m]->instruction = l2Controller ->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[m]->instruction;
                    l2Controller -> mainMemory ->row[address_for_mem->set_or_index]->each_column[m]->data = l2Controller ->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[m]->data;
                    l2Controller -> mainMemory ->row[address_for_mem->set_or_index]->each_column[m]->name_of_instruction_or_data = l2Controller ->l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[m]->name_of_instruction_or_data;
                }
            }
        }
        if(l2Controller -> busL2AndMemory -> is_empty == 0){
            strcpy(l2Controller -> L2_executingInstructions.state, "Memory to L2C : Block");
        }
        else{
            strcpy(l2Controller -> L2_executingInstructions.state, "Memory to L2C");
        }

    }
    else if(strcmp(l2Controller -> L2_executingInstructions.state, "Memory to L2C") == 0){
        if(l2Controller -> busL2AndMemory -> is_empty == 0){
            Address* address_for_memory = Assign_Address_to_Memory(address_char);
            In_a_row* column = l2Controller -> mainMemory -> row[address_for_memory -> set_or_index]->each_column[l2Controller -> busL2AndMemory -> counter];
            if(l2Controller -> busL2AndMemory -> counter != 8){
                strcpy(l2Controller -> L2_executingInstructions.state, "Memory to L2C : Block");
                l2Controller = put_data_into_L2_block(l2Controller, column, address_for_memory, set_six_bit);
            }
            else{
                l2Controller -> l2Cache->pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->address = Assign_Address_to_L2(address_char);
                l2Controller -> busL2AndMemory ->counter = 0;
                l2Controller -> busL2AndMemory -> is_empty =0;
                l2Controller -> busL2AndMemory = initialize_bus_between_L2_Memory();
                strcpy(l2Controller -> L2_executingInstructions.state, "L2C to L1C");
            }
        }
        else{
            strcpy(l2Controller -> L2_executingInstructions.state, "Memory to L2C");
        }
    }
    else if (strcmp(l2Controller -> L2_executingInstructions.state, "Memory to L2C : Block") == 0){
        Address* address_for_memory = Assign_Address_to_Memory(address_char);
        In_a_row* column = l2Controller -> mainMemory -> row[address_for_memory -> set_or_index]->each_column[l2Controller -> busL2AndMemory -> counter];
        if(l2Controller -> busL2AndMemory -> counter != 7){
            strcpy(l2Controller -> L2_executingInstructions.state, "Memory to L2C : Block");
            l2Controller = put_data_into_L2_block(l2Controller, column, address_for_memory, set_six_bit);
        }
        else{
            l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->address = Assign_Address_to_L2(address_char);
            l2Controller -> busL2AndMemory = initialize_bus_between_L2_Memory();
            l2Controller -> busL2AndMemory ->counter = 0;
            l2Controller -> busL2AndMemory -> is_empty =0;
            strcpy(l2Controller -> L2_executingInstructions.state, "L2C to L1C");
        }
    }
    return l2Controller;
}


L2_controller* put_data_into_L2_block(L2_controller* l2Controller, In_a_row* column, Address* address_for_memory, int set_six_bit){
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->valid_bit = 1;
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->dirty_bit = 0;
    Address* address_for_L2_int = Assign_Address_to_L2(column -> address ->address_character);
    l2Controller -> busL2AndMemory -> rows = column;
    l2Controller -> busL2AndMemory -> address = address_for_L2_int;
    l2Controller -> busL2AndMemory -> is_empty = 1;
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[l2Controller -> busL2AndMemory -> counter]->name_of_instruction_or_data =
            l2Controller -> busL2AndMemory -> rows->name_of_instruction_or_data;
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[l2Controller -> busL2AndMemory -> counter]->address = l2Controller -> busL2AndMemory -> address;
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[l2Controller -> busL2AndMemory -> counter]->instruction =
            l2Controller -> busL2AndMemory -> rows->instruction;
    l2Controller -> l2Cache -> pointer_to_sets[set_six_bit]->pointer_to_one_block[0]->each_int[l2Controller -> busL2AndMemory -> counter]->data =
            l2Controller -> busL2AndMemory -> rows->data;
    l2Controller -> busL2AndMemory -> counter += 1;
    return l2Controller;
}

L1_controller* L1_write(L1_controller* l1Controller){
    char* state = l1Controller -> L1_executingInstructions.state;
    char * address_char = decimal_to_binary_conversion(l1Controller -> L1_executingInstructions.instruction->address);
    Address* address_coming = Assign_Address_to_L1(address_char);
    int tag_to_find = address_coming->tag;
    int set_six_bit = address_coming -> set_or_index;
    int offet = address_coming -> offset;
    int value = l1Controller -> L1_executingInstructions.instruction->value;

    if(strcmp(state, "P to L1C") == 0){
        int found = 0;

        for (int j = 0; j< 4 && found == 0; j++){  // find if the data is there in data cache
            if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->address->tag == tag_to_find && l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->valid_bit == 1){
                strcpy(l1Controller -> L1_executingInstructions.state, "L1C to L1D : Hit");
                found += 1;
                break;
            }
        }
        if(found == 0) {           // if it is a miss
            // check in victim cache and write buffers
            int found_in_victim_cache = 0;
            int found_in_write_buffer = 0;
            for(int k = 0; k < 2 ; k++){
                if(l1Controller -> victimCache -> block[k]!= NULL && l1Controller -> victimCache ->block[k]->valid_bit == 1 && l1Controller -> victimCache ->block[k]->address -> tag == tag_to_find){
                    found_in_victim_cache += 1;
                    break;
                }
            }

            for(int k = 0; k < 2 ; k++){
                if(l1Controller -> writeBuffer -> block[k]!= NULL && l1Controller -> writeBuffer ->block[k]->valid_bit == 1 && l1Controller -> writeBuffer ->block[k]->address -> tag == tag_to_find){
                    found_in_write_buffer += 1;
                    break;
                }
            }

            // same way for write buffer

            if (found_in_victim_cache > 0){
                strcpy(l1Controller -> L1_executingInstructions.state, "Victim to L1C");
            }
            else if(found_in_write_buffer > 0 ){
                strcpy(l1Controller -> L1_executingInstructions.state, "Update Write Buffer");
            }
            else{      // its a miss in victim cache and in write buffer
                strcpy(l1Controller -> L1_executingInstructions.state , "L1C to L2C");
            }
        }
    } else if(strcmp(state, "L1C to L1D : Hit") == 0){
        for (int j = 0; j< 4 ; j++){
            if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->address->tag == tag_to_find && l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->valid_bit == 1){
                for(int k = 0; k < (l1Controller -> L1_executingInstructions.instruction->number_of_bytes/4); k++){
                    l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[j]->each_int[offet]-> data = value;
                    offet += 1;
                }
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[j]->dirty_bit = 1;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit] = set_lru_bit(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit], j, "");
                break;
            }
        }
        strcpy(l1Controller -> L1_executingInstructions.state, "Done");
    }
    else if(strcmp(state, "L1C to L2C") == 0){
//            if(executingInstructions[i].instruction->type_of_instruction == 10){
        // It will perform the read operation and try to bring back the block in L1
        enqueue(l1Controller -> L1_read_port_queue, l1Controller -> L1_executingInstructions.instruction);
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state, "Update L1C") == 0){
        Address* address_from_L2 = l1Controller -> l2Controller -> busL1AndL2 -> address;
        Address* parsed_address = Assign_Address_to_L1(address_from_L2 -> address_character);
        l1Controller -> l2Controller -> busL1AndL2 -> is_empty = 0;
        int empty_slot = -1;
        int position = -1;
        for(int j = 0; j < 4; j++){
            if (l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[j]->valid_bit == 0){
                empty_slot = j;
                position = j;
                break;
            }
            else if(l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[j]->valid_bit != 0 && l1Controller->l1_data_Cache->pointer_to_set[parsed_address->set_or_index]->pointer_to_one_block[j]->lru == 4){
                position = j;
            }
        }
        if (empty_slot != -1) {
            for (int b = 0; b < 8; b++) {
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->each_int[b] =
                        l1Controller->l2Controller->busL1AndL2->block->each_int[b];
            }
            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->valid_bit = 1;
            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->dirty_bit = 1;
            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[empty_slot]->address = address_coming;
            l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], empty_slot, "empty");

        } else if (empty_slot == -1) {
            // slot is full, place the block in victim cache or write buffer
            if (l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit ==
                0) {
                int empty_slot_in_victim = -1;
                for (int v = 0; v < 2; v++) {
                    if (l1Controller->victimCache->block[v]->valid_bit != 1) {
                        empty_slot_in_victim = v;
                        break;
                    }
                }
                int position_in_victim = -1;
                if (empty_slot_in_victim == -1) {  // check which block to replace
                    for (int v = 0; v < 2; v++) {
                        if (l1Controller->victimCache->block[v]->lru == 2) {
                            position_in_victim = v;
                        }
                    }
                }
                if (empty_slot_in_victim != -1) {    // victim slot is empty
                    for (int r = 0; r < 8; r++) {
                        l1Controller->victimCache->block[empty_slot_in_victim]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                    }
                    l1Controller->victimCache->block[empty_slot_in_victim]->valid_bit = 1;
                    l1Controller->victimCache->block[empty_slot_in_victim]->dirty_bit = 0;
                    l1Controller->victimCache->block[empty_slot_in_victim]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                    l1Controller->victimCache = set_lru_bit_for_victim_cache(l1Controller->victimCache,
                                                                             empty_slot_in_victim, "empty");

                } else {                       // victim slot is not empty
                    for (int r = 0; r < 8; r++) {
                        l1Controller->victimCache->block[position_in_victim]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                    }
                    l1Controller->victimCache->block[position_in_victim]->valid_bit = 1;
                    l1Controller->victimCache->block[position_in_victim]->dirty_bit = 0;
                    l1Controller->victimCache->block[position_in_victim]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                    l1Controller->victimCache = set_lru_bit_for_victim_cache(l1Controller->victimCache,
                                                                             position_in_victim, "empty");
                }
                //int lru_bit = l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[position]->lru;
                for (int b = 0; b < 8; b++) {
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] =
                            l1Controller->l2Controller->busL1AndL2->block->each_int[b];
                }
                //l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address-> set_or_index]->pointer_to_one_block[position]->lru = lru_bit;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], position, "");
            }
            else{
                int empty_slot_in_write_buffer = -1;
                for (int v = 0; v < 2; v++) {
                    if (l1Controller->writeBuffer->block[v]->valid_bit != 1) {
                        empty_slot_in_write_buffer = v;
                        break;
                    }
                }
                int position_in_write = -1;
                if (empty_slot_in_write_buffer == -1) {  // check which block to replace
                    for (int v = 0; v < 2; v++) {
                        if (l1Controller->writeBuffer->block[v]->lru == 2) {
                            position_in_write = v;
                        }
                    }
                }
                if (empty_slot_in_write_buffer != -1) {    // write buffer is empty
                    for (int r = 0; r < 8; r++) {
                        l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                    }
                    l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->valid_bit = 1;
                    l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->dirty_bit = 1;
                    l1Controller->writeBuffer->block[empty_slot_in_write_buffer]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                    l1Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                             empty_slot_in_write_buffer, "empty");

                } else {                       // write buffer is not empty
                    int position_in_write = -1;
                    if (empty_slot_in_write_buffer == -1) {  // check which block to replace
                        for (int v = 0; v < 2; v++) {
                            if (l1Controller->writeBuffer->block[v]->lru == 2) {
                                position_in_write = v;
                            }
                        }
                    }
                    if(l1Controller -> bus_for_L1_and_write_buffer -> is_empty == 0){
                        for(int w = 0; w < 8; w++){
                            l1Controller -> bus_for_L1_and_write_buffer -> block->each_int[w] = l1Controller->writeBuffer->block[position_in_write]->each_int[w];
                        }
                        l1Controller -> bus_for_L1_and_write_buffer->is_empty = 1;
                        l1Controller->bus_for_L1_and_write_buffer->block->address = l1Controller->writeBuffer->block[position_in_write]->address;
                        l1Controller->bus_for_L1_and_write_buffer->block->valid_bit = 1;
                        l1Controller ->bus_for_L1_and_write_buffer-> block->dirty_bit = 1;
                        l1Controller = update_L2_from_write_buffer(l1Controller);
                    }

                    for (int r = 0; r < 8; r++) {
                        l1Controller->writeBuffer->block[position_in_write]->each_int[r] = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[r];
                    }
                    l1Controller->writeBuffer->block[position_in_write]->valid_bit = 1;
                    l1Controller->writeBuffer->block[position_in_write]->dirty_bit = 1;
                    l1Controller->writeBuffer->block[position_in_write]->address = l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address;
                    l1Controller->writeBuffer = set_lru_bit_for_write_buffer(l1Controller->writeBuffer,
                                                                             position_in_write, "");
                }
                //int lru_bit = l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address -> set_or_index] -> pointer_to_one_block[position]->lru;
                for (int b = 0; b < 8; b++) {
                    l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] =
                            l1Controller->l2Controller->busL1AndL2->block->each_int[b];
                }
                //l1Controller -> l1_data_Cache -> pointer_to_set[parsed_address-> set_or_index]->pointer_to_one_block[position]->lru = lru_bit;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                l1Controller->l1_data_Cache->pointer_to_set[set_six_bit] = set_lru_bit(
                        l1Controller->l1_data_Cache->pointer_to_set[set_six_bit], position, "");

            }
        }
        l1Controller->l2Controller->busL1AndL2 = initialize_bus_between_L1_L2();
        for(int k = 0; k < (l1Controller -> L1_executingInstructions.instruction->number_of_bytes/4); k++){
            l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[offet]-> data = value;
            offet += 1;
        }
        l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 1;
        strcpy(l1Controller -> L1_executingInstructions.state, "Done");
    }
    else if(strcmp(state, "L2C to Memory") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if(strcmp(state, "Memory to L2C") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state, "L2C to L1C") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if (strcmp(state,"Memory to L2C : Block") == 0){
        l1Controller -> l2Controller -> L2_executingInstructions = l1Controller -> L1_executingInstructions;
        l1Controller -> l2Controller = L2_read(l1Controller -> l2Controller);
        l1Controller -> L1_executingInstructions = l1Controller -> l2Controller -> L2_executingInstructions;
    }
    else if(strcmp(state, "Victim to L1C") == 0){
        int found_in_victim_cache = 0;
        int position_in_victim = -1;
        for(int k = 0; k < 2 ; k++){  // find the position where the tag is found
            if(l1Controller -> victimCache -> block[k]!= NULL && l1Controller -> victimCache ->block[k]->valid_bit == 1 && l1Controller -> victimCache ->block[k]->address -> set_or_index == set_six_bit && l1Controller -> victimCache ->block[k]->address -> tag == tag_to_find){
                found_in_victim_cache += 1;
                position_in_victim = k;
                break;
            }
        }
        int replace = 0;
        int position = -1;
        if(found_in_victim_cache > 0){
            // first put the block with lru = 4 from L1 data in the bus and then replace that L1data place with the tag from victim cache
            for(int l = 0; l < 4 ; l++){
                if(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->lru == 4){
                    if(l1Controller -> bus_for_L1_and_victim -> is_empty == 0){
                        replace = 1;
                        position = l;
                        for(int b = 0; b < 8; b++){
                            l1Controller -> bus_for_L1_and_victim -> block->each_int[b] = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->each_int[b];
                        }
                        //l1Controller -> bus_for_L1_and_victim -> block->lru = 0;
                        l1Controller -> bus_for_L1_and_victim ->address = l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[l]->address;
                        l1Controller -> bus_for_L1_and_victim -> is_empty = 1;
                    }
                }
            }
            if(replace == 1){
                for(int b = 0; b < 8; b++){
                    l1Controller -> l1_data_Cache ->pointer_to_set[set_six_bit]->pointer_to_one_block[position]->each_int[b] = l1Controller -> victimCache -> block[position_in_victim]->each_int[b];
                }
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->dirty_bit = 0;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->valid_bit = 1;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[position]->address = address_coming;
                l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit] = set_lru_bit(l1Controller -> l1_data_Cache -> pointer_to_set[set_six_bit], position, "");
                for(int b = 0; b < 8; b++){
                    l1Controller -> victimCache->block[position_in_victim]->each_int[b] = l1Controller -> bus_for_L1_and_victim -> block->each_int[b];
                }
                l1Controller -> victimCache -> block[position_in_victim]->dirty_bit = 0;
                l1Controller -> victimCache -> block[position_in_victim]->valid_bit = 1;
                l1Controller -> victimCache -> block[position_in_victim]->address = l1Controller -> bus_for_L1_and_victim -> address;
                l1Controller -> victimCache = set_lru_bit_for_victim_cache(l1Controller -> victimCache, position_in_victim, "");
                l1Controller -> bus_for_L1_and_victim = initialize_bus_between_L1_L2();
                strcpy(l1Controller -> L1_executingInstructions.state, "L1C to L1D and P");
            }
        }
    }
    else if(strcmp(state, "Update Write Buffer") == 0) {  // merge the writes in write buffer
        int found_in_write_buffer = 0;
        int position_in_buffer = -1;
        for (int k = 0; k < 2; k++) {  // find the position where the tag is found
            if (l1Controller->writeBuffer->block[k] != NULL && l1Controller->writeBuffer->block[k]->valid_bit == 1 &&
                l1Controller->writeBuffer->block[k]->address->set_or_index == set_six_bit &&
                l1Controller->writeBuffer->block[k]->address->tag == tag_to_find) {
                found_in_write_buffer += 1;
                position_in_buffer = k;
                break;
            }
        }
        if(found_in_write_buffer > 0){
            for(int k = 0; k < (l1Controller -> L1_executingInstructions.instruction->number_of_bytes/4); k++){
                l1Controller -> writeBuffer -> block[position_in_buffer]->each_int[offet]-> data = value;
                offet += 1;
            }
            l1Controller -> writeBuffer -> block[position_in_buffer]->dirty_bit = 1;
            strcpy(l1Controller -> L1_executingInstructions.state, "Done");
        }

    }
    return l1Controller;
}



