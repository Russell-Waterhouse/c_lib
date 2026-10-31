#ifndef JSON_H
#define JSON_H

#include "../types/strings.h"
#include "../types/types.h"
#include <stdbool.h>

typedef struct JsonType JsonType;
typedef struct JsonArray JsonArray;
typedef struct JsonKV JsonKV;

typedef enum {
  JSON_value_type_Int,
  JSON_value_type_Float,
  JSON_value_type_String,
  JSON_value_type_Boolean,
  JSON_value_type_Array,
  JSON_value_type_Object,
} ValueType;

typedef struct JsonValue {
  ValueType type;
  char *key;
  size_t key_len;
  struct JsonValue *first_child;
  struct JsonValue *next_sibling;
  union {
    i64 int_val;
    double float_val;
    char *str_val;
    bool bool_val;
  };
} JsonValue;

typedef struct Json {
  JsonValue *values;
  size_t values_len;
  size_t values_capacity;
  char *strs;
  size_t strs_len;
  size_t strs_capacity;
} Json;

Json parse(char *json_str, size_t json_str_len);
String stringify(Json json);

#endif // JSON_H
