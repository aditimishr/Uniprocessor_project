//
// Created by aditimishra on 5/26/2019.
//

#include <stdlib.h>
#include "Address.h"
#include "stdio.h"
#include "string.h"

char *Get_string_from_start_to_end(char *address, int i, int i1, char *output);

Address* Assign_Address_to_L1(char* bits_for_address){
    char* output = malloc(7);
    Get_string_from_start_to_end(bits_for_address,0,6,output);
    char* TagString = output;
    output = malloc(7);
    Get_string_from_start_to_end(bits_for_address,6,6,output);
    char* set_or_index_bit = output;
    output = malloc(6);
    Get_string_from_start_to_end(bits_for_address,12,5,output);
    char* OffsetString = output;
    int Tag = convert_binary_to_decimal(TagString);
    int Set_or_Index = convert_binary_to_decimal(set_or_index_bit);
    int Offset = convert_binary_to_decimal(OffsetString);
    int decimal_value = convert_binary_to_decimal(bits_for_address);

    Address* address = malloc(sizeof(Address));
    address->tag = Tag;
    address->set_or_index = Set_or_Index;
    address->offset = Offset;
    strcpy(address->address_character,bits_for_address);
    address -> decimal_address = decimal_value;
    return address;
}

Address* Assign_Address_to_L2(char* bits_for_address){
    char* output = malloc(4);
    Get_string_from_start_to_end(bits_for_address,0,3,output);
    char* TagString = output;
    output = malloc(10);
    Get_string_from_start_to_end(bits_for_address,3,9,output);
    char* set_or_index_bit = output;
    output = malloc(6);
    Get_string_from_start_to_end(bits_for_address,12,5,output);
    char* OffsetString = output;
    int Tag = convert_binary_to_decimal(TagString);
    int Set_or_Index = convert_binary_to_decimal(set_or_index_bit);
    int Offset = convert_binary_to_decimal(OffsetString);
    int decimal_value = convert_binary_to_decimal(bits_for_address);

    Address* address = malloc(sizeof(Address));
    address->tag = Tag;
    address->set_or_index = Set_or_Index;
    address->offset = Offset;
    strcpy(address->address_character,bits_for_address);
    address -> decimal_address = decimal_value;
    return address;
}

Address* Assign_Address_to_Memory(char* bits_for_address){
    char* output = malloc(10);
    Get_string_from_start_to_end(bits_for_address,0,12,output);
    char* set_or_index_bit = output;
    output = malloc(6);
    Get_string_from_start_to_end(bits_for_address,12,5,output);
    char* OffsetString = output;
    int Tag = -1;
    int Set_or_Index = convert_binary_to_decimal(set_or_index_bit);
    int Offset = convert_binary_to_decimal(OffsetString);
    int decimal_value = convert_binary_to_decimal(bits_for_address);

    Address* address = malloc(sizeof(Address));
    address->tag = Tag;
    address->set_or_index = Set_or_Index;
    address->offset = Offset;
    strcpy(address->address_character,bits_for_address);
    address -> decimal_address = decimal_value;
    return address;
}

char *Get_string_from_start_to_end(char *all_characters, int start, int length, char *substring) {
    memcpy( substring, &all_characters[start], length );
    substring[length] = '\0';
    return substring;
}

char* decimal_to_binary_conversion(int decimal_number){
    int total_bits = 17;
    char *s = malloc(total_bits+1);  // for \0 in c
    s[total_bits] = '\0';
    unsigned int u = *(unsigned int*)&decimal_number;
    int i;
    unsigned int mask = 1 << (total_bits-1); // fill in values right-to-left
    for (i = 0; i < total_bits; i++, mask >>= 1)
        s[i] = ((u & mask) != 0) + '0';
    return s;
}

int convert_binary_to_decimal(char* any_character){
    char* end;
    int value = strtol(any_character,&end,2);
    return value;
}

