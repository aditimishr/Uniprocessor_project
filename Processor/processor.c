//
// Created by aditimishra on 5/28/2019.
//

#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "processor.h"

processor* processor1_global;

int store_the_instruction_command_in_the_queue(){
    FILE* file_pointer;
    file_pointer = fopen("D:\\ACA\\Mishra_Aditi_CSCI8510_Project\\Uniprocessor\\Input_File.txt", "r");
    char type_of_instruction[6];
    int address;
    if(file_pointer == NULL){
        printf("Input_File.txt file failed to open");
    }
    else{
        while(fscanf(file_pointer, "%s %d\n", type_of_instruction, &address) != EOF) {
//            printf("%s", type_of_instruction);
//            printf("%d", address);
            Instruction* instruction = initialize_instruction(NULL, 10, address, -1, -1);
            enqueue(processor1_global -> cpu_Instruction_Queue, instruction);
        }
        fclose(file_pointer) ;
    }
}

void run_CPU(L1_Cache* L1_instruction_cache, L1_Cache* L1_data_cache, L2_Cache* L2_cache){

    processor* processor1 = malloc(sizeof(processor));
    // enter the instruction read from the input file into the queue
    processor1 -> cpu_Instruction_Queue = createQueue(1000);
    processor1 -> cpu_Data_Request_Queue = createQueue(1000);
    processor1 -> file_instruction_complete = 1;
    processor1 -> l1_controller = initialize_L1_controller(L1_instruction_cache, L1_data_cache, L2_cache);
    processor1 -> cpu_clock_cycle = 0;
    processor1 -> length_of_executing_instruction = 0;
    processor1_global = processor1;
    store_the_instruction_command_in_the_queue();

    // After having file instruction in the cpu_Instruction_Queue, read those one by one

    read_file_instruction();
}

int read_file_instruction(){
    while(! isEmpty(processor1_global -> cpu_Instruction_Queue)){
        printf("\n");
        printf("Reading instructions from L1_instruction_cache");
        printf("\n");
        if(isEmpty(processor1_global -> cpu_Data_Request_Queue)){
            Instruction* instruction_to_execute = dequeue(processor1_global -> cpu_Instruction_Queue);
            //printf("\n%d\n", instruction_to_execute->address);
            //printf("%s", &instruction_to_execute);

            processor1_global -> length_of_executing_instruction = 0;
            for(int i = 0; i < 8; i++){
                processor1_global -> executing_instructions[i] = iniatialize_Executing_instructions();
            }
            // perform L1_read operation
            CPU_Instruction_read(instruction_to_execute);
        }
    }
}

void CPU_Instruction_read(Instruction* instruction_to_L1){

    // we already know that instructions are there in L1 instruction cache
    // so no need to check whether it is in the cache or not
    // just enqueue it and cpu will start executing those instructions
    int address = instruction_to_L1 -> address;
    int block_address = address/32;
    int set_address = address/128;
    L1_Cache* l1_instruction_Cache = processor1_global -> l1_controller ->l1_instruction_Cache;
    Block* particular_block = l1_instruction_Cache->pointer_to_set[set_address]->pointer_to_one_block[block_address];
    for(int i = 0; i < 8; i++){
        if(particular_block -> each_int[i]->name_of_instruction_or_data!=NULL){
            enqueue(processor1_global -> cpu_Data_Request_Queue, particular_block->each_int[i]->instruction); // enqueue the processor's queue with the
            // whole block having the same block address
        }
    }
    check_for_instructions();
}

void check_for_instructions(){                // check if processor has any instruction to execute
    processor1_global -> cpu_clock_cycle = 0;
    Queue* cpu_queue = processor1_global -> cpu_Data_Request_Queue;
    Instruction* dequeue_instruction = NULL;
    int first_time = 0;
    while(processor1_global -> length_of_executing_instruction > 0 || first_time == 0){
        int count = 0;
        processor1_global -> cpu_clock_cycle += 1;
        printf("CPU clock cycle : %d\n", processor1_global -> cpu_clock_cycle);
        if(!isEmpty(cpu_queue)){
            dequeue_instruction = dequeue(cpu_queue);
            processor1_global -> length_of_executing_instruction += 1;
            strcpy(processor1_global -> executing_instructions[processor1_global -> length_of_executing_instruction - 1].state, "P to L1C");
            processor1_global -> executing_instructions[processor1_global -> length_of_executing_instruction - 1].instruction = dequeue_instruction;
        }
        printf("\n");
        for(int i = processor1_global -> length_of_executing_instruction - 1; i >= 0 ; i--){
            if(strcmp(processor1_global -> executing_instructions[i].state, "Done") == 0 || strcmp(processor1_global -> executing_instructions[i].state, "L1C to P") == 0){
                strcpy(processor1_global -> executing_instructions[i].state, "Done");
                count += 1;
                Address* address_coming = Assign_Address_to_L1(decimal_to_binary_conversion(processor1_global -> executing_instructions[i].instruction->address));
                int tag_to_find = address_coming->tag;
                int set_six_bit = address_coming -> set_or_index;
                int offet = address_coming -> offset;
                int data_offset = offet / 4;
                int data1 = -1;
                printf("%s : %s\t", processor1_global -> executing_instructions[i].state, processor1_global->executing_instructions[i].instruction->name_of_instruction_or_data);
                for(int j = 0; j < 4 ; j++){
                    if (processor1_global -> l1_controller -> l1_data_Cache -> pointer_to_set[set_six_bit]->pointer_to_one_block[j]->address->tag == tag_to_find){
                        for(int l = 0; l <  (processor1_global -> executing_instructions[i].instruction->number_of_bytes / 4) ; l++){
                            data1 = processor1_global -> l1_controller->l1_data_Cache->pointer_to_set[set_six_bit]->pointer_to_one_block[j]->each_int[data_offset]->data;
                            printf("%d\t", data1);
                            data_offset = data_offset + 1;
                        }
                        break;
                    }
                }
                if(data1 == -1){
                    printf("Tag removed due to conflict miss\t");
                }
                if(processor1_global -> executing_instructions[i].instruction->type_of_instruction == 10){
                    printf("%s\t %d\n", "CPURead", processor1_global -> executing_instructions[i].instruction->address);
                }
                else{
                    printf("%s\t %d\t %d\n", "CPUWrite", processor1_global -> executing_instructions[i].instruction->address, processor1_global -> executing_instructions[i].instruction->value);
                }
            }
            else{
                if(processor1_global -> executing_instructions[i].instruction->type_of_instruction == 10){
                    printf("%s : %s\t %s\t %d\n", processor1_global -> executing_instructions[i].state, processor1_global->executing_instructions[i].instruction->name_of_instruction_or_data, "CPURead", processor1_global -> executing_instructions[i].instruction->address);
                }
                else{
                    printf("%s : %s\t %s\t %d\t %d\n", processor1_global -> executing_instructions[i].state, processor1_global->executing_instructions[i].instruction->name_of_instruction_or_data, "CPUWrite", processor1_global -> executing_instructions[i].instruction->address, processor1_global -> executing_instructions[i].instruction->value);
                }
            }
        }

        printf("\n");
        if(count == processor1_global -> length_of_executing_instruction){
            processor1_global -> length_of_executing_instruction = 0;
        }
        if(processor1_global -> length_of_executing_instruction > 0){
            for(int k = 0; k < processor1_global -> length_of_executing_instruction ; k++){
                processor1_global -> l1_controller ->L1_executingInstructions = processor1_global -> executing_instructions[k];
                // check if the current instruction index and tag matches with the previous instruction's tag and index

                int waiting_state = 0;
                if(strcmp(processor1_global -> executing_instructions[k].state, "Wait")== 0 || strcmp(processor1_global -> executing_instructions[k].state, "P to L1C")== 0){
                    for (int l = 0; l < k ; l++){
                        char* previous_address = decimal_to_binary_conversion(processor1_global -> executing_instructions[l].instruction->address);
                        char* current_address = decimal_to_binary_conversion(processor1_global -> executing_instructions[k].instruction->address);
                        Address* previous = Assign_Address_to_L1(previous_address);
                        Address* current = Assign_Address_to_L1(current_address);
                        int position = -1;
                        if(previous->tag == current->tag && previous->set_or_index == current->set_or_index){
                            for(int r = 0; r < 4; r++){
                                if(processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->valid_bit == 1 && processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->address->tag == current->tag && processor1_global -> executing_instructions[l].instruction->type_of_instruction == 10){
                                    position = r;
                                    strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "P to L1C");
                                    break;
                                }
                                else if(processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->valid_bit == 0 && processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->address->tag == current->tag && processor1_global -> executing_instructions[l].instruction->type_of_instruction == 10){
                                    position = r;
                                    waiting_state += 1;
                                    strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "Wait");
                                    break;
                                }
                                else if(processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->valid_bit == 1 && processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->address->tag == current->tag && processor1_global -> executing_instructions[l].instruction->type_of_instruction == 20 && strcmp(processor1_global -> executing_instructions[l].state, "Done") != 0){
                                    position = r;
                                    waiting_state += 1;
                                    strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "Wait");
                                    break;
                                }
                                else if(processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->valid_bit == 1 && processor1_global -> l1_controller -> l1_data_Cache->pointer_to_set[previous->set_or_index]->pointer_to_one_block[r]->address->tag == current->tag && processor1_global -> executing_instructions[l].instruction->type_of_instruction == 20 && strcmp(processor1_global -> executing_instructions[l].state, "Done") == 0){
                                    position = r;
                                    strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "P to L1C");
                                    break;
                                }
                                else if(strcmp(processor1_global -> executing_instructions[l].state, "Done") == 0){
                                    position = r;
                                    strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "P to L1C");
                                    break;
                                }
                            }
                            if(position == -1){
                                waiting_state += 1;
                                strcpy(processor1_global -> l1_controller ->L1_executingInstructions.state, "Wait");
                                break;
                            }
                        }
                    }
                }

                if(waiting_state == 0){
                    if(processor1_global -> executing_instructions->instruction[k].type_of_instruction == 10){
                        processor1_global -> l1_controller = L1_read(processor1_global -> l1_controller);
                    }
                    else{
                        processor1_global -> l1_controller = L1_write(processor1_global -> l1_controller);
                    }
                }
                processor1_global -> executing_instructions[k] = processor1_global -> l1_controller -> L1_executingInstructions;
            }
        }
        first_time += 1;
    }
}