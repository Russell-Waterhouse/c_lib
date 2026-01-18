#include "dynamic_array.h"

GENERIC_TYPEDynArr GENERIC_TYPE_dyn_arr_initialize(size_t size) {
  GENERIC_TYPEDynArr a;
  a.size = size;
  a.memsize = size;
  a.arr = calloc(size, sizeof(size_t));
  if (NULL == a.arr) {
    puts("Failed to allocate requested memory for array");
    exit(-1);
  }

  return a;
}

GENERIC_TYPEDynArr GENERIC_TYPE_insert_back_or_die(GENERIC_TYPEDynArr a, GENERIC_TYPE value) {
  if (a.size >= a.memsize) {
    if (a.memsize == 0) {
      a.memsize = DYNAMIC_ARRAY_START_SIZE;
    } else {
      a.memsize *= 2;
    }
    a.arr = realloc(a.arr, a.memsize * sizeof(GENERIC_TYPE));
    if (a.arr == NULL) {
      printf("Failed to allocate memory for array holder\n");
      exit(-1);
    }
  }

  a.arr[a.size++] = value;

  return a;
}

GENERIC_TYPE GENERIC_TYPE_at_or_die(GENERIC_TYPEDynArr a, size_t index) {
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

void GENERIC_TYPE_free(GENERIC_TYPEDynArr a) {
  if (NULL == a.arr) {
    return;
  }

  free(a.arr);
  a.arr = NULL;
}

u8 GENERIC_TYPE_equal(GENERIC_TYPEDynArr a1, GENERIC_TYPEDynArr a2) {
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

