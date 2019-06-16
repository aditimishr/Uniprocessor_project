//
// Created by aditimishra on 6/9/2019.
//

#include "Bus_L2_Memory.h"
#include "malloc.h"
#include "stdio.h"

Bus_L2_and_Memory* initialize_bus_between_L2_Memory(){
    Bus_L2_and_Memory* bus = malloc(sizeof(Bus_L2_and_Memory));
    bus -> rows = NULL;
    bus -> is_empty = 0;
    bus -> address = NULL;
    bus -> counter = 0;
}