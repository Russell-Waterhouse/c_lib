#ifndef GENERIC_TYPE_DYNAMIC_ARRAY_H
#define GENERIC_TYPE_DYNAMIC_ARRAY_H

#include "../types/types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFINE_DYNAMIC_ARRAY(T, Name)                                          \
  typedef struct {                                                             \
    size_t size;                                                               \
    size_t capacity;                                                           \
    T *arr;                                                                    \
  } Name;                                                                      \
  static inline Name Name##_init(size_t size) {                                \
    Name a;                                                                    \
    a.size = size;                                                             \
    a.capacity = size;                                                         \
    a.arr = (T *)calloc(size, sizeof(char));                                   \
    if (NULL == a.arr) {                                                       \
      puts("Failed to allocate requested memory for array");                   \
      exit(-1);                                                                \
    }                                                                          \
                                                                               \
    return a;                                                                  \
  }                                                                            \
                                                                               \
  static inline void Name##_insert_back_or_die(Name *a, T value) {             \
    if (a->size >= a->capacity) {                                              \
      if (a->capacity == 0) {                                                  \
        a->capacity = DYNAMIC_ARRAY_START_SIZE;                                \
      } else {                                                                 \
        a->capacity *= 2;                                                      \
      }                                                                        \
      a->arr = (T *)realloc(a->arr, a->capacity * sizeof(T));                  \
      if (a->arr == NULL) {                                                    \
        printf("Failed to allocate memory for array holder\n");                \
        exit(-1);                                                              \
      }                                                                        \
    }                                                                          \
                                                                               \
    a->arr[a->size++] = value;                                                 \
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
  static inline void Name##_resize(Name *a, size_t new_size) {                 \
    if (a->capacity > new_size) {                                              \
      puts("Cannot resize a dynamic array smaller than it currently is");      \
      exit(-1);                                                                \
    }                                                                          \
    a->capacity = new_size;                                                    \
    a->arr = (T *)realloc(a->arr, a->capacity * sizeof(T));                    \
    if (a->arr == NULL) {                                                      \
      printf("Failed to allocate memory for array holder\n");                  \
      exit(-1);                                                                \
    }                                                                          \
  }

#endif
