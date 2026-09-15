#include "../io/json.h"
#include "../core/debugging.h"
#include "../core/pretty_print.h"
#include "../types/types.h"
#include <string.h>

Status test_nested_resize_array() {
  char *expected =
      "[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,"
      "27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,"
      "51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,"
      "75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,"
      "99,100]";
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

Status test_nested_non_emtpy_object_float() {
  char *expected = "{\"foo\":{\"bar\":1.125000}}";
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

Status test_nested_non_emtpy_object_int() {
  char *expected = "{\"foo\":{\"bar\":123}}";
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

Status test_two_keys_object() {
  char *expected = "{\"x1\":1,\"x2\":2}";
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

Status test_nested_empty_object() {
  char *expected = "{\"foo\":{}}";
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
  debugger();
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
  debugger();
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
  debugger();
  arena_free(json.arena);
  free(result.str);
  return FAIL;
}

void test_json() {
  puts("Starting json tests.");
  Status (*tests_arr[])(void) = {
      test_empty_json,
      test_single_object_int,
      test_single_object_float,
      test_empty_array,
      test_single_element_array,
      test_basic_array,
      test_nested_empty_object,
      test_nested_non_emtpy_object_int,
      test_nested_non_emtpy_object_float,
  };

  size_t len = sizeof(tests_arr) / sizeof(tests_arr[0]);

  for (size_t i = 0; i < len; i++) {
    Status res = tests_arr[i]();
    printf("Test number %zu was %d\n", i, res);
    if (SUCCESS != tests_arr[i]()) {
      print_red("There were JSON test failures.");
      return;
    }
  }
  print_green("Tests completed successfully!");
  return;
}
