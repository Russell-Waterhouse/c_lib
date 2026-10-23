#include "json.h"
#include "../core/debugging.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define JSON_ARRAY_INITIAL_LEN 256
#define JSON_OBJ_INITIAL_LEN 256
#define STRINGIFY_INITIAL_LEN 256
#define STACK_BUFFER_LEN 256
#define BASE_TEN 10

// a billion
#define MAX_CHARS_FOR_STRINGS 1000000000
#define MAX_NUM_JSON_ELEMENTS 1000000000
#define MAX_CHARS_IN_JSON_STRING 1000000000

typedef struct {
  char buffr[STACK_BUFFER_LEN];
  size_t len;
} StackBuffer;

// TODO: use in JsonStack
/*
typedef struct {
  JsonValue *parent;
  JsonValue *last_sibling;
}JsonFrame;
*/

typedef struct {
  JsonValue *stack[STACK_BUFFER_LEN];
  size_t len;
} JsonStack;

void push_stack_buffer(char c, StackBuffer *buffr) {
  if (buffr->len + 1 > STACK_BUFFER_LEN) {
    puts("buffer overrun");
    exit(1);
  }
  buffr->buffr[buffr->len] = c;
  buffr->len++;
}

void push_JsonStack(JsonValue *j, JsonStack *js) {
  if (js->len + 1 > STACK_BUFFER_LEN) {
    puts("json stack buffer overrun");
    exit(1);
  }
  js->stack[js->len] = j;
  js->len++;
};

void push_JsonValue(Json *json) {
  if (json->values_len + 1 > json->values_capacity) {
    puts("Json capacity overrun");
    exit(1);
  }
  json->values_len++;
  json->values[json->values_len] = (JsonValue){0};
}

JsonValue *get_parent(JsonStack *parent_stack) {
  if (parent_stack->len == 0) {
    puts("called get_parent on an empty stack");
    exit(1);
  }
  return parent_stack->stack[parent_stack->len - 1];
}

char pop_stack_buffr(StackBuffer *buffr) {
  if (buffr->len < 1) {
    puts("buffer under run");
    exit(1);
  }
  char c = buffr->buffr[buffr->len - 1];
  buffr->len--;
  return c;
}

void add_char_to_stack_buffer(StackBuffer *scratch, char c) {
  if (scratch->len + 1 >= STACK_BUFFER_LEN) {
    puts("something too long for scratch buffer");
    exit(1);
  }
  scratch->buffr[scratch->len] = c;
  scratch->len += 1;
}

long int flush_stack_buffer_to_int(StackBuffer *scratch) {
  add_char_to_stack_buffer(scratch, '\0');
  scratch->len = 0;
  return strtol(scratch->buffr, NULL, BASE_TEN);
  // TODO: there's more to strtol for error handling
}

float flush_stack_buffer_to_float(StackBuffer *scratch) {
  add_char_to_stack_buffer(scratch, '\0');
  scratch->len = 0;
  return strtof(scratch->buffr, NULL);
  // TODO: there's more to strtof for error handling
}

typedef enum {
  StartState,
  ReadingObjectKey,
  ReadingObjectKeyString,
  ReadingObjectValueInt,
  ReadingObjectValueFloat,
  ReadyToReadValue,
  EscapedInKeyString,
  EscapedInValueString,
  ReadingValueKeyString,
  FinishedReadingObject,
  FinishedReadingObjectKey,
  FinishedReadingObjectValue,
  ReadingArrayElement,
  ReadingArrayElementInt,
  FinishedReadingArray,
} ParserStateMachineState;

Json parse(char *json_str, size_t json_str_len) {
  Json json = {0};
  json.strs_capacity = MAX_CHARS_FOR_STRINGS;
  json.values_capacity = MAX_NUM_JSON_ELEMENTS;
  json.strs = malloc(json.strs_capacity * sizeof(char));
  json.values = malloc(json.values_capacity * sizeof(JsonValue));
  StackBuffer scratch = {0};
  ParserStateMachineState state = StartState;
  JsonStack parent_stack = {0};
  // JsonStack sibling_stack = {0}; // TODO: siblings

  json.values[json.values_len] = (JsonValue){0};
  for (size_t i = 0; i < json_str_len; i++) {
    char c = json_str[i];
    if (isspace(c)) {
      continue;
    }
    switch (c) {
    case '{': {
      if (StartState == state) {
        // {"foo":123}
        // ^
        json.values[json.values_len].type = JSON_value_type_Object;
        state = ReadingObjectKey;
        push_JsonStack(&json.values[json.values_len], &parent_stack);
        push_JsonValue(&json);
        continue;
      }
      if (ReadyToReadValue == state) {
        // {"foo":{"bar":123}}
        //        ^
        state = ReadingObjectKey;
        continue;
      }
    } break;
    case '}': {
      JsonValue *parent = get_parent(&parent_stack);
      if (JSON_value_type_Object != parent->type) {
        printf("unmatched curly brace at charcter %zu\n", i);
        exit(1);
      }
      if (ReadingObjectKey == state) {
        // {}
        //  ^
        // this object has no key and no value
        // TODO
        state = FinishedReadingObject;
        parent_stack.len--;
        continue;
      }
      if (ReadingObjectValueInt == state) {
        // {"key":123}
        //           ^
        long int value = flush_stack_buffer_to_int(&scratch);
        json.values[json.values_len].type = JSON_value_type_Int;
        json.values[json.values_len].int_val = value;
        parent->len++;
        if (parent->first_child == NULL) {
          parent->first_child = &json.values[json.values_len];
        }
        parent_stack.len--;
        push_JsonValue(&json);
        state = FinishedReadingObject;
        continue;
      }
      if (ReadingObjectValueFloat == state) {
        // {"key":1.23}
        //            ^
        float value = flush_stack_buffer_to_float(&scratch);
        json.values[json.values_len].type = JSON_value_type_Float;
        json.values[json.values_len].float_val = value;
        JsonValue *parent = get_parent(&parent_stack);
        parent->len++;
        if (parent->first_child == NULL) {
          parent->first_child = &json.values[json.values_len];
        }
        parent_stack.len--;
        push_JsonValue(&json);
        state = FinishedReadingObject;
        continue;
      }
    } break;
    case '[': {
      if (state == StartState) {
        json.values[json.values_len].type = JSON_value_type_Array;
        push_JsonStack(&json.values[json.values_len], &parent_stack);
        push_JsonValue(&json);
        state = ReadingArrayElement;
        continue;
      }
    } break;
    case ']': {
      JsonValue *parent = get_parent(&parent_stack);
      if (JSON_value_type_Array != parent->type) {
        puts("unmatched brackes");
        exit(1);
      }
      if (ReadingArrayElement == state) {
        // this array has no values
        state = FinishedReadingArray;
        parent_stack.len--;
        continue;
      }
      if (ReadingArrayElementInt == state) {
        // [123, 456]
        //          ^
        long int value = flush_stack_buffer_to_int(&scratch);
        json.values[json.values_len].type = JSON_value_type_Int;
        json.values[json.values_len].int_val = value;
        parent->len++;
        if (parent->first_child == NULL) {
          parent->first_child = &json.values[json.values_len];
        }
        parent_stack.len--;
        push_JsonValue(&json);
        state = FinishedReadingObject;
        continue;
        state = FinishedReadingArray;
        parent_stack.len--;
        break;
      }
    case '"': {
      if (state == ReadingObjectKeyString) {
        state = FinishedReadingObjectKey;
        continue;
      }
      if (state == ReadingObjectKey || state == FinishedReadingObjectValue) {
        state = ReadingObjectKeyString;
        // pointer to next character is this JsonObject key
        json.values[json.values_len].key = &json.strs[json.strs_len];
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      exit(1);
    } break;
    case ',': {
      if (ReadingArrayElementInt == state) {
        // TODO
        continue;
      }
      if (ReadingObjectValueInt == state) {
        // long int value = flush_stack_buffer_to_int(&scratch);
        // TODO
        state = FinishedReadingObjectValue;
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      exit(1);
    } break;
    case ':': {
      if (FinishedReadingObjectKey == state) {
        state = ReadyToReadValue;
      } else {
        puts("unhandled");
        exit(1);
      }
      continue;
    } break;
    default: {
      if (ReadyToReadValue == state && isdigit(c)) {
        state = ReadingObjectValueInt;
      }
      if (ReadingArrayElementInt == state && isdigit(c)) {
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      if (ReadingArrayElement == state && isdigit(c)) {
        state = ReadingArrayElementInt;
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      if ((ReadingObjectValueInt == state ||
           ReadingObjectValueFloat == state) &&
          isdigit(c)) {
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      if (ReadingObjectKeyString == state) {
        // reading object key, copy it into strs array.
        json.strs[json.strs_len] = c;
        json.strs_len++;
        json.values[json.values_len].key_len++;
        continue;
      }
      if (ReadingObjectValueInt == state && c == '.') {
        state = ReadingObjectValueFloat;
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      debugger();
      exit(1);
    } break;
    }
    }
  }

  if (parent_stack.len > 0) {
    puts("Unclosed characters in JSON");
    debugger();
    exit(1);
  }

  return json;
}

void copy_int_to_string(String *s, long int v) {
  char buffr[STACK_BUFFER_LEN] = {0};
  snprintf(buffr, STACK_BUFFER_LEN, "%ld", v);
  size_t str_buffr_size = strlen(buffr);
  if (s->size + str_buffr_size > s->capacity) {
    String_resize(s, (s->capacity * 2) + str_buffr_size);
  }
  memcpy(&s->arr[s->size], buffr, str_buffr_size);
  s->size += str_buffr_size;
}

void copy_float_to_string(String *s, float f) {
  char buffr[STACK_BUFFER_LEN] = {0};
  snprintf(buffr, STACK_BUFFER_LEN, "%f", f);
  size_t str_buffr_size = strlen(buffr);
  if (s->size + str_buffr_size > s->capacity) {
    puts("TODO: resize here float to string");
  }
  memcpy(&s->arr[s->size], buffr, str_buffr_size);
  s->size += str_buffr_size;
}

void copy_key_and_colon_to_string(String *s, char *key, size_t key_len) {
  // + 2 for the characters `"":`
  if (s->size + key_len + 2 >= s->capacity) {
    puts("over capacity");
    exit(0);
  }
  String_insert_back_or_die(s, '"');
  memcpy(&s->arr[s->size], key, key_len);
  s->size += key_len;
  String_insert_back_or_die(s, '"');
  String_insert_back_or_die(s, ':');
}

String stringify(Json json) {
  if (json.values[0].type != JSON_value_type_Object &&
      json.values[0].type != JSON_value_type_Array) {
    puts("Cannot stringify invalid json");
    exit(1);
  }

  String s = {0};
  s.arr = malloc(MAX_CHARS_IN_JSON_STRING);
  s.size = 0;
  s.capacity = MAX_CHARS_IN_JSON_STRING;

  if (json.values[0].type == JSON_value_type_Object) {
    String_insert_back_or_die(&s, '{');
  }

  if (json.values[0].type == JSON_value_type_Array) {
    String_insert_back_or_die(&s, '[');
  }

  JsonStack parent_stack = {0};
  push_JsonStack(&json.values[0], &parent_stack);
  JsonValue *curr = json.values[0].first_child;
  while (curr != NULL) {
    ValueType parent_type = parent_stack.stack[parent_stack.len - 1]->type;
    switch (curr->type) {
    case JSON_value_type_Object: {
    } break;
    case JSON_value_type_Array: {
    } break;
    case JSON_value_type_Float: {
      if (parent_type == JSON_value_type_Object) {
        copy_key_and_colon_to_string(&s, curr->key, curr->key_len);
      }
      copy_float_to_string(&s, curr->float_val);
      curr = curr->next_sibling;
    } break;
    case JSON_value_type_Int: {
      if (parent_type == JSON_value_type_Object) {
        copy_key_and_colon_to_string(&s, curr->key, curr->key_len);
      }
      copy_int_to_string(&s, curr->int_val);
      curr = curr->next_sibling;
    } break;
    case JSON_value_type_Boolean: {
    } break;
    case JSON_value_type_String: {
    } break;
    default:
      puts("Should never be here");
      exit(1);
    }
  }

  while (parent_stack.len > 0) {
    JsonValue *parent = get_parent(&parent_stack);
    if (parent->type == JSON_value_type_Object) {
      String_insert_back_or_die(&s, '}');
    } else if (parent->type == JSON_value_type_Array) {
      String_insert_back_or_die(&s, ']');
    } else {
      puts("Invalid parent type");
      exit(1);
    }
    parent_stack.len--;
  }

  return s;
}
