#ifndef JSON_H
#define JSON_H

#include "../types/strings.h"
#include "../types/types.h"
#include <stdbool.h>

typedef struct JsonType JsonType;
typedef struct JsonArray JsonArray;
typedef struct JsonObject JsonObject;

typedef enum {
  JSON_value_type_Int,
  JSON_value_type_Float,
  JSON_value_type_String,
  JSON_value_type_boolean,
  JSON_value_type_Array,
  JSON_value_type_Object,
} ValueType;

// We're just being lazy and using a fat struct here, any field that isn't
// defined as used by .type will just be garbage.
typedef struct {
  ValueType type;

  long int int_val;
  float float_val;
  String str_val;
  bool bool_val;

  struct JsonArray *array_val;
  struct JsonObject *object_val;
} JsonValue;

struct JsonArray {
  JsonValue *values;
  u32 len;
  u32 capacity;
};

struct JsonObject {
  String key;
  JsonValue value;
};

typedef enum {
  JsonStartObject,
  JsonStartArray,
} JsonStart;

typedef struct {
  JsonStart start;
  JsonObject obj;
  JsonArray arr;
  Arena *arena;
} Json;

Json parse(char *json_str, size_t json_str_len);
String stringify(Json json);

#endif // JSON_H
