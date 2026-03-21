#include <erl_nif.h>
#include "dynamic_array.h"

int dynamic_array_init(DynamicArray *array, int initial_size) {
  array->array = malloc(sizeof(ERL_NIF_TERM) * initial_size);
  array->size = initial_size;
  array->used = 0;

  if (array->array == NULL)
    return -1;
  else
    return 0;
}

int dynamic_array_append(DynamicArray *array, ERL_NIF_TERM term) {
  if (array->used < array->size) {
    array->array[array->used] = term;
    array->used++;

    return 0;
  } else {
    int new_size = array->size * 2;
    void *reallocd = realloc(array->array, sizeof(ERL_NIF_TERM) * new_size);
    if (reallocd == NULL)
      return -1;
    
    array->array = reallocd;
    array->size = new_size;
    return dynamic_array_append(array, term);
  }
}
