//
// Created by aditimishra on 6/9/2019.
//

#ifndef UNIPROCESSOR_BUS_L1_L2_H
#define UNIPROCESSOR_BUS_L1_L2_H

// The bus between L1 and L2 can hold one block at a time

#include "../Block/Block.h"

typedef struct Bus1{
    Block* block;
    int is_empty;    // to check whether the bus is empty to transfer the block
    Address* address;
} Bus_L1_and_L2;

Bus_L1_and_L2* initialize_bus_between_L1_L2();


#endif //UNIPROCESSOR_BUS_L1_L2_H
