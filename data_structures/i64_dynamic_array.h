#ifndef i64_DYNAMIC_ARRAY_H
#define i64_DYNAMIC_ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../types/types.h"



typedef struct {
  size_t size;
  size_t memsize;
  i64* arr;
} i64DynArr;

i64DynArr i64_insert_back_or_die(i64DynArr a, i64 value);
i64 i64_at_or_die(i64DynArr a, size_t index);
void i64_free(i64DynArr a);
i64DynArr i64_dyn_arr_initialize(size_t size);
u8 i64_equal(i64DynArr a1, i64DynArr a2);

#endif
