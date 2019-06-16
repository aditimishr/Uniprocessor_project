//
// Created by aditimishra on 6/9/2019.
//

#include "Victim_Cache.h"
#include "malloc.h"
#include "stdio.h"

Victim_Cache* initialize_victim_cache(){
    Victim_Cache* victimCache = malloc(sizeof(Victim_Cache));
    Address* address = Assign_Address_to_L1("00000000000000000");
    for(int b = 0; b < 2; b++){
        victimCache -> block[b] = initialize_L1_block(address, 0);
    }
    return victimCache;
}