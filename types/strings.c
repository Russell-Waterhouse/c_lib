#ifndef MY_STRINGS_H
#define MY_STRINGS_H

#include <stddef.h>
#include "./types.h"
#include "implementations/size_t_dynamic_array.h"
#include "implementations/i64_dynamic_array.h"
#include "./arenas.h"

typedef struct {
  size_t size;
  size_t memsize;
  char* str;
} String;

typedef struct {
  size_t memsize;
  size_t size;
  String* arr;
} DynStringArr;

typedef struct {
  Result status;
  DynStringArr strs;
} SplitResultOption;

typedef struct {
  Result status;
  String slice;
} SliceResult;

typedef struct {
  Result status;
  String str;
  Error err;
} StrResult;

typedef struct {
  Result status;
  DynStringArr dyn_arr;
  Error err;
} DynStrArrResult;


/* save optional/result type functions */
String str_at(DynStringArr a, size_t index);
StrResult u64_to_str(u64 v);
StrResult concat(String s1, String s2);
DynStrArrResult insert_back(DynStringArr a, String value);
StrResult cstr_to_str(const char* cstr, u64 size);
StrResult cstr_to_str_unsafe(const char* cstr);
StrResult cstr_to_str_arena_unsafe(Arena* arena, const char* cstr);
StrResult cstrs_to_str_unsafe(char** cstr);
IOResult free_str(String s);
SliceResult slice(String s1, u64 start, u64 end);
SplitResultOption split_str(String s, char split_char);
u8 str_equal(String s1, String s2);
u64Result str_to_u64(String s);
String trim(String s);
u64 blank(String s);
i64 find_first(String s, String search_str);
i64DynArr make_kmp_fail_table(String w);
SizeTDynArr find_all(String s, String search_str);
u8 ends_with(String s, String search_str);
u8 starts_with(String s, String search_str);

/* not yet implemented functions */
u64 replace_first(String s, String search_str, String replacement_str);
u64 replace_all(String s, String search_str, String replacement_str);

/* _in_place functions */
Result strip_in_place(String s);

#endif /* MY_STRINGS_H */

#ifdef  MY_STRINGS_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "./types.h"
#include "./implementations/size_t_dynamic_array.h"
#include "implementations/i64_dynamic_array.h"
#include "./casts.h"

/* TODO: don't include the .c files, make those header-only libraries */
#include "./implementations/size_t_dynamic_array.c"
#include "implementations/i64_dynamic_array.c"

StrResult cstr_to_str(const char* cstr, u64 size) {
  StrResult res = {0};
  u64 i;

  res.str.str = (char*)calloc(size, sizeof(char));
  if (NULL == res.str.str) {
    res.status = FAIL;
    res.err.code = ERR_MEM_ALLOC_FAIL;
    res.err.msg = "Failed to allocate memory for string in __FUNCTION__";
    return res;
  }

  for(i = 0; i < size; i++) {
    res.str.str[i] = cstr[i];
  }
  res.status = SUCCESS;
  res.str.size = size;
  res.str.memsize = size;

  return res;
}


StrResult cstr_to_str_unsafe(const char* cstr) {
  return cstr_to_str(cstr, strlen(cstr));
}

StrResult cstr_to_str_arena_unsafe(Arena* arena, const char* cstr) {
  StrResult res;
  u64 i;
  size_t size = strlen(cstr);
  size_t memsize = sizeof(char) * size;
  PointerResult p = arena_push(arena, memsize);
  if (p.status != SUCCESS) {
    res.status = FAIL;
    res.err.code = p.val.err.code;
    res.err.msg = p.val.err.msg;
    return res;
  }
  res.str.str = (char*)p.val.res;
  for(i = 0; i < size; i++) {
    res.str.str[i] = cstr[i];
  }
  res.status = SUCCESS;
  res.str.size = size;
  res.str.memsize = memsize;
  return res;
}


IOResult free_str(String s) {
  IOResult res = {0};
  if (NULL == s.str) {
    res.status = FAIL;
    res.err.code = ERR_INVALID_ARG;
    res.err.msg = "Attempting to free a null string at __LINE__";
    return res;
  }
  free(s.str);
  res.status = SUCCESS;
  return res;
}

u8 str_equal(String s, String s2) {
  u64 i;
  if (s.size != s2.size) {
    return 0;
  }
  for(i = 0; i < s.size; i++) {
    if (s.str[i] != s2.str[i]) {
      return 0;
    }
  }
  return 1;
}

StrResult concat(String s1, String s2) {
  u64 i, j;
  StrResult s = {0};
  u64 size;
  if (
    (s1.size > 0 && NULL == s1.str) ||
    (s2.size > 0 && NULL == s2.str)
  ) {
    s.status = FAIL;
    s.err.code = ERR_INVALID_ARG;
    s.err.msg = "String with non-zero size has null string at concat";
    return s;
  }
  size = s1.size + s2.size;

  s.str.str = (char*)calloc(size, sizeof(char));
  if (NULL == s.str.str) {
    s.status = FAIL;
    s.err.code = ERR_MEM_ALLOC_FAIL;
    s.err.msg = "Failed to allocate memory when concatenating strings";
    return s;
  }

  for(i = 0; i < s1.size; i++) {
    s.str.str[i] = s1.str[i];
  }
  for(j = 0; j < s2.size; j++, i++) {
    s.str.str[i] = s2.str[j];
  }

  return s;
}

u8 ends_with(String s, String search_str) {
  size_t i;
  if (NULL == s.str || NULL == search_str.str) {
    return 0;
  }

  if (search_str.size > s.size) {
    return 0;
  }

  for (i = search_str.size; i > 0; i--) {
    if (search_str.str[i] != s.str[i]) {
      return 0;
    }
  }

  return 1;
}

u8 starts_with(String s, String search_str) {
  size_t i;
  if (NULL == s.str || NULL == search_str.str) {
    return 0;
  }

  if (search_str.size > s.size) {
    return 0;
  }

  for (i = 0; i > search_str.size; i++) {
    if (search_str.str[i] != s.str[i]) {
      return 0;
    }
  }

  return 1;
}

i64DynArr make_kmp_fail_table(String w) {
  i64 pos, cnd;
  i64DynArr t = i64_dyn_arr_initialize(w.size + 1);
  pos = 1;
  cnd = 0;

  t.arr[0] = -1;
  while (pos < w.size) {
    /* since the values of pos and cnd are controlled by an algorithm,
     * we make unchecked casts here */
    u64 pos_u64 = (u64)pos;
    u64 cnd_u64 = (u64)cnd;
    if (w.str[pos_u64] == w.str[cnd_u64]) {
      t.arr[pos] = t.arr[cnd];
    } else {
      t.arr[pos] = cnd;
      while (cnd >= 0 && w.str[pos_u64] != w.str[(u64)cnd]) {
        cnd = t.arr[cnd];
      }
    }
    ++pos;
    ++cnd;
  }
  t.arr[pos] = cnd;

  return t;
}

/* substring search returning the index of the first result
 * in a string, or -1 if the search_str was not found in s.
 * Uses the Knuth–Morris–Pratt (KMP) algorithm.
 */
i64 find_first(String s, String search_str) {
  i64 j, k;
  i64DynArr t = {0};
  /*initializing t */
  t = make_kmp_fail_table(search_str);
  if (s.size < search_str.size || NULL == s.str || NULL == search_str.str) {
    return -1;
  }

  j = 0;
  k = 0;
  while (j < s.size) {
    if (search_str.str[(u64)k] == s.str[(u64)j]) {
      j++;
      k++;
      if (k == search_str.size) {
        return j - k;
      }
    } else {
      k = t.arr[(u64)k];
      if (k < 0) {
        j++;
        k++;
      }
    }
  }

  return -1;
}

/* substring search returning an array of indexes to all substring matches
 * in a string, or -1 if the search_str was not found in s.
 * Uses the Knuth–Morris–Pratt (KMP) algorithm.
 */
SizeTDynArr find_all(String s, String search_str) {
  i64 j, k;
  i64DynArr t = {0};
  /*initializing t */
  t = make_kmp_fail_table(search_str);
  SizeTDynArr found_positions = {0};
  if (s.size < search_str.size || NULL == s.str || NULL == search_str.str) {
    return found_positions;
  }

  /* Since j and k are controlled by known algorithms, we make unchecked
   * casts with them */
  j = 0;
  k = 0;
  while (j < s.size) {
    if (search_str.str[(u64)k] == s.str[(u64)j]) {
      j++;
      k++;
      if (k == search_str.size) {
        found_positions = size_t_insert_back_or_die(found_positions, (u64)(j - k));
        k = t.arr[(u64)k];
      }
    } else {
      k = t.arr[(u64)k];
      if (k < 0) {
        j++;
        k++;
      }
    }
  }


  return found_positions;
}

u64 replace_first(String s, String search_str, String replacement_str);
u64 replace_all(String s, String search_str, String replacement_str);


SliceResult slice(String s, u64 start, u64 end) {
  SliceResult res;
  u64 i;
  u64 size;
  String slice = {0};

  if (start > end || NULL == s.str) {
    res.status = FAIL;
    return res;
  }
  size = end - start;

  slice.str = (char*)calloc(size, sizeof(char));
  if (NULL == slice.str) {
    puts("Failed to allocate memory for string slice");
    res.status = FAIL;
    return res;
  }

  for(i = 0; i < end - start; i++) {
    slice.str[i] = s.str[i+start];
  }
  slice.size = size;
  slice.memsize = size;
  res.status = SUCCESS;
  res.slice = slice;
  return res;
}

SplitResultOption split_str(String s, char split_char) {
  SplitResultOption res;
  DynStrArrResult strs = {0};
  u64 i;
  u64 start;

  start = 0;
  for (i = 0; i < s.size; i++) {
    if (s.str[i] == split_char) {
      SliceResult slice_result = slice(s, start, i);
      if (slice_result.status == FAIL) {
        res.status = FAIL;
        return res;
      }
      strs = insert_back(strs.dyn_arr, slice_result.slice);
      if (strs.status != SUCCESS) {
        res.status = FAIL;
        return res;
      }
      start = i + 1;
    }
  }
  if (start != i) {
      SliceResult slice_result = slice(s, start, i);
      if (slice_result.status == FAIL) {
        res.status = FAIL;
        return res;
      }
      strs = insert_back(strs.dyn_arr, slice_result.slice);
      if (strs.status != SUCCESS) {
        res.status = FAIL;
        return res;
      }
      start = i + 1;
  }
  res.status = SUCCESS;
  res.strs = strs.dyn_arr;
  return res;
}


String trim(String s);

u64 blank(String s) {
  u64 i;
  if (NULL == s.str) {
    return 1;
  }
  for (i = 0; i < s.size; i++) {
    if (s.str[i] != ' ' && s.str[i] != '\t' && s.str[i] != '\0' && s.str[i] != '\n') {
      return 0;
    }
  }
  return 1;
}

u64Result str_to_u64(String s) {
  u64Result res;
  u64 result = 0;
  if (s.size == 0 || NULL == s.str) {
    res.status = FAIL;
    res.err.code = ERR_INVALID_ARG;
    res.err.msg = "Cannot cast a null string to u64";
    return res;
  }

  u64 i;
  for (i = 0; i < s.size; i++) {
    char c = s.str[i];
    if (c < '0' || c > '9') {
      printf("Failed to parse ascii code %d\n", c);
      res.status = FAIL;
      return res;
    }
    result *= 10;
    /* This can never be negative so we're not checking for errors with this cast */
    u64Result res = i64_to_u64(c - '0');
    if (res.status == FAIL) {
    }
    result += res.res;
  }

  res.status = SUCCESS;
  res.res = result;
  return res;
}


StrResult u64_to_str(u64 v) {
  /* u64 has a max of 20 digits */
  char str[32];
  sprintf(str, "%lu", v);
  return cstr_to_str(str, strlen(str));
}


Result strip_in_place(String s) {
  /* TODO: Finish the rest of this */
  if (s.str[s.size - 1] == '\n') {
    s.size--;
  }

  return SUCCESS;
}

DynStrArrResult insert_back(DynStringArr a, String value) {
  DynStrArrResult res;
  if (a.size >= a.memsize) {
    if (a.memsize <= a.size) {
      a.memsize = DYNAMIC_ARRAY_START_SIZE;
    } else {
      a.memsize *= 2;
    }
    a.arr = (String*)realloc(a.arr, a.memsize);
    if (a.arr == NULL) {
      res.status = FAIL;
      res.err.code = ERR_MEM_ALLOC_FAIL;
      return res;
    }
  }

  a.arr[a.size++] = value;
  res.dyn_arr = a;
  res.status = SUCCESS;
  return res;
}

String at(DynStringArr a, size_t index) {
  if (index >= a.size) {
    printf("Attempted to index an array outside of its size. Exiting the program.");
    exit(-1);
  }

  return a.arr[index];
}

#endif /*  MY_STRINGS_IMPLEMENTATION */
