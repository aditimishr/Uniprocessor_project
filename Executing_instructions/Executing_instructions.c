//
// Created by aditimishra on 6/3/2019.
//

#include "Executing_instructions.h"
#include "malloc.h"
#include "stdio.h"

struct Executing_instructions iniatialize_Executing_instructions(){
    Executing_instructions* executingInstructions = malloc(sizeof(Executing_instructions));
    executingInstructions -> instruction = NULL;
    executingInstructions -> state = NULL;
}