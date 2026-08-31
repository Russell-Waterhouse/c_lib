#ifndef JSON_H
#define JSON_H

#include "../types/strings.h"

// this is a mess but I'm coding on mobile

typedef enum {
  Int,
  Float,
  String,
  Array,
  Object,
} ValueType;

typedef struct {
  JsonValue* values,
  u32 len,
  u32 capacity,
} JsonArray;

typedef union JsonValue {
  int,
  float,
  String,
  JsonArray*,
  JsonObject,
};
  
typedef struct {
  String key,
  ValueType value_type,
} JsonObject;

typedef struct {
  JsonObject obj,
  Arena *arena,
} Json;


#endif // JSON_H