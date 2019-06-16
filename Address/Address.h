//
// Created by aditimishra on 5/26/2019.
//

#ifndef UNIPROCESSOR_ADDRESS_H
#define UNIPROCESSOR_ADDRESS_H

typedef struct Address_container{
    int tag;
    int set_or_index;
    int offset;
    int decimal_address;
    char address_character[18];
} Address;

Address* Assign_Address_to_L1(char* address_character);

Address* Assign_Address_to_L2(char* address_character);

Address* Assign_Address_to_Memory(char* address_character);

char* decimal_to_binary_conversion(int decimal_number);

int convert_binary_to_decimal(char* any_character);


#endif //UNIPROCESSOR_ADDRESS_H
