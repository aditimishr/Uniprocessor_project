//
// Created by aditimishra on 6/9/2019.
//

#include "Write_Buffer.h"
#include "malloc.h"
#include "stdio.h"

Write_Buffer* initialize_write_buffer(){
    Write_Buffer* writeBuffer = malloc(sizeof(Write_Buffer));
    Address* address = Assign_Address_to_L1("00000000000000000");
    for(int b = 0; b < 2; b++){
        writeBuffer -> block[b] = initialize_L1_block(address, 0);
    }
    return writeBuffer;
}