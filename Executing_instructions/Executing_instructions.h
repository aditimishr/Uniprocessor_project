//
// Created by aditimishra on 6/3/2019.
//

#ifndef UNIPROCESSOR_EXECUTING_INSTRUCTIONS_H
#define UNIPROCESSOR_EXECUTING_INSTRUCTIONS_H

#include "../Instruction/Instruction.h"

typedef struct Executing_instructions{
    char* state;
    Instruction* instruction;
} Executing_instructions;

struct Executing_instructions iniatialize_Executing_instructions();



#endif //UNIPROCESSOR_EXECUTING_INSTRUCTIONS_H
