#ifndef GENERIC_TYPE_DYNAMIC_ARRAY_H
#define GENERIC_TYPE_DYNAMIC_ARRAY_H

#include "../types/types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFINE_DYNAMIC_ARRAY(T, Name)                                          \
  typedef struct {                                                             \
    size_t size;                                                               \
    size_t memsize;                                                            \
    T *arr;                                                                    \
  } Name;                                                                      \
  static inline Name Name##_init(size_t size) {                                \
    Name a;                                                                    \
    a.size = size;                                                             \
    a.memsize = size;                                                          \
    a.arr = calloc(size, sizeof(size_t));                                      \
    if (NULL == a.arr) {                                                       \
      puts("Failed to allocate requested memory for array");                   \
      exit(-1);                                                                \
    }                                                                          \
                                                                               \
    return a;                                                                  \
  }                                                                            \
                                                                               \
  Name Name##_insert_back_or_die(Name a, T value) {                            \
    if (a.size >= a.memsize) {                                                 \
      if (a.memsize == 0) {                                                    \
        a.memsize = DYNAMIC_ARRAY_START_SIZE;                                  \
      } else {                                                                 \
        a.memsize *= 2;                                                        \
      }                                                                        \
      a.arr = realloc(a.arr, a.memsize * sizeof(T));                           \
      if (a.arr == NULL) {                                                     \
        printf("Failed to allocate memory for array holder\n");                \
        exit(-1);                                                              \
      }                                                                        \
    }                                                                          \
                                                                               \
    a.arr[a.size++] = value;                                                   \
                                                                               \
    return a;                                                                  \
  }                                                                            \
                                                                               \
  void Name##_free(Name a) {                                                   \
    if (NULL == a.arr) {                                                       \
      return;                                                                  \
    }                                                                          \
                                                                               \
    free(a.arr);                                                               \
    a.arr = NULL;                                                              \
  }                                                                            \
                                                                               \
  u8 Name##_equal(Name a1, Name a2) {                                          \
    size_t i;                                                                  \
                                                                               \
    if (a1.size != a2.size) {                                                  \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    for (i = 0; i < a1.size; i++) {                                            \
      if (a1.arr[i] != a2.arr[i]) {                                            \
        return 0;                                                              \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }

#endif
