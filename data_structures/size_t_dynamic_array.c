#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "./size_t_dynamic_array.h"



SizeTDynArr size_t_dyn_arr_initialize(size_t size) {
  SizeTDynArr a;
  a.size = size;
  a.memsize = size;
  a.arr = calloc(size, sizeof(size_t));
  if (NULL == a.arr) {
    puts("Failed to allocate requested memory for array");
    exit(-1);
  }

  return a;
}

SizeTDynArr size_t_insert_back_or_die(SizeTDynArr a, size_t value) {
  if (a.size >= a.memsize) {
    if (a.memsize == 0) {
      a.memsize = DYNAMIC_ARRAY_START_SIZE;
    } else {
      a.memsize *= 2;
    }
    a.arr = realloc(a.arr, a.memsize * sizeof(size_t));
    if (a.arr == NULL) {
      printf("Failed to allocate memory for array holder\n");
      exit(-1);
    }
  }

  a.arr[a.size++] = value;

  return a;
}

size_t size_t_at_or_die(SizeTDynArr a, size_t index) {
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

void size_t_free(SizeTDynArr a) {
  if (NULL == a.arr) {
    return;
  }

  free(a.arr);
  a.arr = NULL;
}
