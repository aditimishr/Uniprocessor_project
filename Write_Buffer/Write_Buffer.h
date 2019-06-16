//
// Created by aditimishra on 6/9/2019.
//

#ifndef UNIPROCESSOR_WRITE_BUFFER_H
#define UNIPROCESSOR_WRITE_BUFFER_H

#include "../Block/Block.h"

typedef struct Write_buffer{
    Block* block[2];
} Write_Buffer;

Write_Buffer* initialize_write_buffer();



#endif //UNIPROCESSOR_WRITE_BUFFER_H
