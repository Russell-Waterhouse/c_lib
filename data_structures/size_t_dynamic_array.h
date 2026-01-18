#ifndef SIZE_T_DYNAMIC_ARRAY_H
#define SIZE_T_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../types/types.h"


typedef struct {
  size_t size;
  size_t memsize;
  size_t* arr;
} SizeTDynArr;

SizeTDynArr size_t_insert_back_or_die(SizeTDynArr a, size_t value);
size_t size_t_at_or_die(SizeTDynArr a, size_t index);
void size_t_free(SizeTDynArr a);
SizeTDynArr size_t_dyn_arr_initialize(size_t size);

#endif
