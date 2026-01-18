#include <stdio.h>
#include "../data_structures/dynamic_array.h"
#include "../types/types.h"
#include "../core/pretty_print.h"

#define one_million (u64)1000000
#define expected_memsize (u64)0b01 << 20

Result test_insert_back_when_empty() {
  GENERIC_TYPEDynArr arr = {0};
  arr = GENERIC_TYPE_insert_back_or_die(arr, 100);
  if (arr.memsize == DYNAMIC_ARRAY_START_SIZE && arr.size == 1 && arr.arr[0] == 100) {
    GENERIC_TYPE_free(arr);
    return SUCCESS;
  }

  print_red("Failed to insert back");
  GENERIC_TYPE_free(arr);
  return FAIL;
}

Result test_resizing() {
  GENERIC_TYPEDynArr a = {0};
  u64 i;
  for (i = 0; i < one_million; i++) {
    a = GENERIC_TYPE_insert_back_or_die(a, i);
  }
  if (a.memsize == expected_memsize && a.size == one_million) {
    for (i = 0; i < one_million; i++) {
      if (GENERIC_TYPE_at_or_die(a, i) != i) {
        print_red("Resizing failed");
        GENERIC_TYPE_free(a);
        return FAIL;
      }
    }

    GENERIC_TYPE_free(a);
    return SUCCESS;
  } else {
    print_red("Resizing failed to have the expected memsize and size");
    GENERIC_TYPE_free(a);
    return FAIL;
  }
}

void test_dynamic_array() {
  puts("Starting dynamic array tests.");
  if (
      test_insert_back_when_empty() == SUCCESS &&
      test_resizing() == SUCCESS
  ) {
    print_green("Tests completed successfully!");
    return;
  }


  print_red("There were test failures.");
  return;
}
