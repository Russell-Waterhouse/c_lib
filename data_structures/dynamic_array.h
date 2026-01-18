#ifndef GENERIC_TYPE_DYNAMIC_ARRAY_H
#define GENERIC_TYPE_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../types/types.h"



typedef u64 GENERIC_TYPE;

typedef struct {
  size_t size;
  size_t memsize;
  GENERIC_TYPE* arr;
} GENERIC_TYPEDynArr;

GENERIC_TYPEDynArr GENERIC_TYPE_insert_back_or_die(GENERIC_TYPEDynArr a, GENERIC_TYPE value);
GENERIC_TYPE GENERIC_TYPE_at_or_die(GENERIC_TYPEDynArr a, size_t index);
void GENERIC_TYPE_free(GENERIC_TYPEDynArr a);
GENERIC_TYPEDynArr GENERIC_TYPE_dyn_arr_initialize(size_t size);
u8 GENERIC_TYPE_equal(GENERIC_TYPEDynArr a1, GENERIC_TYPEDynArr a2);

#endif 
