#include "../io/json.h"
#include "../core/debugging.h"
#include "../core/pretty_print.h"
#include "../types/types.h"
#include <string.h>

// Status test_basic_array() {
//   char *actual = "[1,2,3]";
//   size_t size = strlen(actual);
//   Json json = parse(actual, size);
//   String result = stringify(json);
//   if (size == result.size && !memcmp(actual, result.str, result.size)) {
//     arena_free(json.arena);
//     free(result.str);
//     return SUCCESS;
//   }
//   arena_free(json.arena);
//   free(result.str);
//   return FAIL;
// }


Status test_empty_array() {
  char *actual = "[]";
  size_t size = strlen(actual);
  Json json = parse(actual, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(actual, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}


Status test_single_object_float() {
  char *actual = "{\"key\":1.100000}";
  size_t size = strlen(actual);
  Json json = parse(actual, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(actual, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_single_object_int() {
  char *actual = "{\"key\":12345}";
  size_t size = strlen(actual);
  Json json = parse(actual, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(actual, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_empty_json() {
  char *empty = "{}";
  size_t size = strlen(empty);
  Json json = parse(empty, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(empty, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

void test_json() {
  puts("Starting json tests.");
  if (SUCCESS == test_empty_json() && SUCCESS == test_single_object_int() &&
      SUCCESS == test_single_object_float() && SUCCESS == test_empty_array()) {
    print_green("Tests completed successfully!");
    return;
  }

  print_red("There were JSON test failures.");
  return;
}
