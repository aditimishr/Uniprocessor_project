//
// Created by aditimishra on 5/26/2019.
//

#ifndef UNIPROCESSOR_PROCESSOR_H
#define UNIPROCESSOR_PROCESSOR_H


#include "../Controller/Controller.h"
#include "../Queue/Queue.h"

typedef struct Processor{
    int file_instruction_complete;     // this variable will tell cpu that instruction reading is complete
    // its time to read next instruction from the input file
    Queue* cpu_Instruction_Queue;
    Queue* cpu_Data_Request_Queue;
    L1_controller* l1_controller;
    Executing_instructions executing_instructions[8];
    int cpu_clock_cycle;
    int length_of_executing_instruction;
} processor;

void run_CPU(L1_Cache* L1_instruction_cache, L1_Cache* L1_data_cache, L2_Cache* L2_cache);

int store_the_instruction_command_in_the_queue();

int read_file_instruction();

void CPU_Instruction_read(Instruction* instruction_to_L1);

void check_for_instructions();

void CPU_instruction_execution();


#endif //UNIPROCESSOR_PROCESSOR_H
