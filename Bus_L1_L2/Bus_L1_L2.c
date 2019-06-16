//
// Created by aditimishra on 6/9/2019.
//


#include "Bus_L1_L2.h"
#include "malloc.h"
#include "stdio.h"

Bus_L1_and_L2* initialize_bus_between_L1_L2(){
    Bus_L1_and_L2* bus = malloc(sizeof(Bus_L1_and_L2));
    Address* address = Assign_Address_to_L1("00000000000000000");
    bus -> block = initialize_L1_block(address, 0);
    bus -> is_empty = 0;
    bus -> address = address;
}

