#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../types.h"
#include "./i64_dynamic_array.h"


i64DynArr i64_dyn_arr_initialize(size_t size) {
  i64DynArr a;
  a.size = size;
  a.memsize = size;
  a.arr = calloc(size, sizeof(size_t));
  if (NULL == a.arr) {
    puts("Failed to allocate requested memory for array");
    exit(-1);
  }

  return a;
}

i64DynArr i64_insert_back_or_die(i64DynArr a, i64 value) {
  if (a.size >= a.memsize) {
    if (a.memsize == 0) {
      a.memsize = DYNAMIC_ARRAY_START_SIZE;
    } else {
      a.memsize *= 2;
    }
    a.arr = realloc(a.arr, a.memsize * sizeof(i64));
    if (a.arr == NULL) {
      printf("Failed to allocate memory for array holder\n");
      exit(-1);
    }
  }

  a.arr[a.size++] = value;

  return a;
}

i64 i64_at_or_die(i64DynArr a, size_t index) {
  if (a.arr == NULL) {
    printf("Passed a null dynamic array to the 'at' function. Exiting the program.\n");
    exit(-1);
  }
  if (index >= a.size) {
    printf("Attempted to index an array outside of its size %lu > %lu. Exiting the program.\n", index, a.size);
    /* TODO: print size, line number, index, callstack here. */
    exit(-1);
  }

  return a.arr[index];
}

void i64_free(i64DynArr a) {
  if (NULL == a.arr) {
    return;
  }

  free(a.arr);
  a.arr = NULL;
}

u8 i64_equal(i64DynArr a1, i64DynArr a2) {
  size_t i;

  if (a1.size != a2.size) {
    return 0;
  }

  for(i = 0; i < a1.size; i++) {
    if (a1.arr[i] != a2.arr[i]) {
      return 0;
    }
  }
  return 1;
}
