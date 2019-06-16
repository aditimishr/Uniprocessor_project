//
// Created by aditimishra on 6/9/2019.
//

#ifndef UNIPROCESSOR_VICTIM_CACHE_H
#define UNIPROCESSOR_VICTIM_CACHE_H

#include "../Block/Block.h"

typedef struct Victim_cache_1{
    Block* block[2];
} Victim_Cache;

Victim_Cache* initialize_victim_cache();

#endif //UNIPROCESSOR_VICTIM_CACHE_H
