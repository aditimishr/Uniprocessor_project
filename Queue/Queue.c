//
// Created by aditimishra on 6/1/2019.
//

#include <zconf.h>
#include "Queue.h"
#include "malloc.h"
#include "stdio.h"

Queue* createQueue(unsigned capacity){
    Queue* queue = malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->instruction = (Instruction*) malloc(queue->capacity * sizeof(Instruction));
    return queue;
}

int isFull(Queue* queue)
{  return (queue->size == queue->capacity);  }


int isEmpty(Queue* queue)
{  return (queue->size == 0); }


void enqueue(Queue* queue, Instruction* instruction)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->instruction[queue->rear] = *instruction;
    queue->size = queue->size + 1;
}

Instruction* dequeue(Queue* queue)
{
    Instruction* ins = NULL;
    if (isEmpty(queue))
        return ins;
    Instruction* instruction = &queue->instruction[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return instruction;
}

// Function to get front of queue
Instruction* front(Queue* queue)
{
    Instruction* ins = NULL;
    if (isEmpty(queue))
        return ins;
    return &queue->instruction[queue->front];
}

// Function to get rear of queue
Instruction* rear(Queue* queue)
{
    Instruction* ins = NULL;
    if (isEmpty(queue))
        return ins;
    return &queue->instruction[queue->rear];
}