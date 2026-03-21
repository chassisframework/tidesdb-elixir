#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <erl_nif.h>

typedef struct {
    int size;
    int used;
    size_t term_size;
    ERL_NIF_TERM *array; 
} DynamicArray;

int dynamic_array_init(DynamicArray *array, int initial_size);
int dynamic_array_append(DynamicArray *array, ERL_NIF_TERM term);

#endif // DYNAMIC_ARRAY_H_
