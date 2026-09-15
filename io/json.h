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

// We're just being lazy and using a fat struct here, any field that isn't
// defined as used by .type will just be garbage.
typedef struct JsonValue {
  ValueType type;
  long int int_val;
  float float_val;
  String str_val;
  bool bool_val;
  size_t len;
  size_t capacity;
  struct JsonValue *arr_values; // only when value type is array
  JsonKV *obj_pairs; // only when value type is object
} JsonValue;

struct JsonKV {
  String key;
  JsonValue value;
};

typedef struct {
  JsonValue value;
  Arena *arena;
} Json;

Json parse(char *json_str, size_t json_str_len);
String stringify(Json json);

#endif // JSON_H
