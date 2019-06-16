//
// Created by aditimishra on 6/9/2019.
//

#ifndef UNIPROCESSOR_BUS_L2_MEMORY_H
#define UNIPROCESSOR_BUS_L2_MEMORY_H

#include "../In_a_row/In_a_row.h"

typedef struct Bus2{
    In_a_row* rows;
    int is_empty;    // to check whether the bus is empty to transfer the block
    Address* address;
    int counter;
} Bus_L2_and_Memory;

Bus_L2_and_Memory* initialize_bus_between_L2_Memory();



#endif //UNIPROCESSOR_BUS_L2_MEMORY_H
