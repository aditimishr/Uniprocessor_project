//
// Created by aditimishra on 6/1/2019.
//

#ifndef UNIPROCESSOR_QUEUE_H
#define UNIPROCESSOR_QUEUE_H

#include "../Instruction/Instruction.h"

typedef struct Queue_for_instruction{ // this code is taken from geeksforgeeks.org
    int front, rear, size;
    unsigned capacity;
    Instruction* instruction;
} Queue;

Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, Instruction* instruction);
Instruction* dequeue(Queue* queue);
Instruction* front(Queue* queue);
Instruction* rear(Queue* queue);


#endif //UNIPROCESSOR_QUEUE_H
