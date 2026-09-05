#include "../io/json.h"
#include "../core/debugging.h"
#include "../core/pretty_print.h"
#include "../types/types.h"
#include <string.h>

Status test_basic_array() {
  char *expected = "[1,2,3]";
  size_t size = strlen(expected);
  Json json = parse(expected, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(expected, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_single_element_array() {
  char *expected = "[1]";
  size_t size = strlen(expected);
  Json json = parse(expected, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(expected, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  debugger();
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_empty_array() {
  char *expected = "[]";
  size_t size = strlen(expected);
  Json json = parse(expected, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(expected, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  debugger();
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_single_object_float() {
  char *expected = "{\"key\":1.100000}";
  size_t size = strlen(expected);
  Json json = parse(expected, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(expected, result.str, result.size)) {
    arena_free(json.arena);
    free(result.str);
    return SUCCESS;
  }
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

Status test_single_object_int() {
  char *expected = "{\"key\":12345}";
  size_t size = strlen(expected);
  Json json = parse(expected, size);
  String result = stringify(json);
  if (size == result.size && !memcmp(expected, result.str, result.size)) {
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
      SUCCESS == test_single_object_float() && SUCCESS == test_empty_array() &&
      SUCCESS == test_single_element_array()) {
    print_green("Tests completed successfully!");
    return;
  }

  debugger();
  print_red("There were JSON test failures.");
  return;
}
