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

typedef struct {
  char buffr[STACK_BUFFER_LEN];
  size_t len;
} StackBuffer;

void push_stack_buffer(char c, StackBuffer *buffr) {
  if (buffr->len + 1 > STACK_BUFFER_LEN) {
    puts("buffer overrun");
    exit(1);
  }
  buffr->buffr[buffr->len] = c;
  buffr->len++;
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

char *flush_scratch_buffer_string_to_arena(StackBuffer *scratch, Arena *arena) {
  char *dest = (char *)(arena_push(arena, scratch->len).val.res);
  memcpy(dest, scratch->buffr, scratch->len);
  scratch->len = 0;
  return dest;
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
  json.arena = arena_create(MiB(16)).arena;
  // todo: look up max number of characters allowed in a number in JSON
  StackBuffer scratch = {0};
  StackBuffer state_stack = {0};
  ParserStateMachineState state = StartState;
  JsonValue *current_value = NULL;
  for (size_t i = 0; i < json_str_len; i++) {
    char c = json_str[i];
    if (isspace(c)) {
      continue;
    }
    switch (c) {
    case '{':
      push_stack_buffer(c, &state_stack);
      if (StartState == state) {
        // {"foo":123}
        // ^
        json.value.type = JSON_value_type_Object;
        state = ReadingObjectKey;
        current_value = &json.value;
        json.value.capacity = JSON_OBJ_INITIAL_LEN;
        json.value.obj_pairs =
            arena_push(json.arena, sizeof(JsonValue) * json.value.capacity)
                .val.res;
        json.value.len = 0;
        continue;
      }
      if (ReadyToReadValue == state) {
        // {"foo":{"bar":123}}
        //        ^
        state = ReadingObjectKey;
        current_value->obj_pairs[current_value->len].value.type =
            JSON_value_type_Object;
        current_value->obj_pairs[current_value->len].value.capacity =
            JSON_OBJ_INITIAL_LEN;
        current_value->obj_pairs[current_value->len].value.obj_pairs =
            arena_push(json.arena, sizeof(JsonValue) * json.value.capacity)
                .val.res;
        current_value->obj_pairs[current_value->len].value.len = 0;
        current_value->len++;

        current_value = &current_value->obj_pairs[0].value;
        continue;
      }
      break;
    case '}':
      if ('{' != pop_stack_buffr(&state_stack)) {
        printf("unmatched curly brace at charcter %zu\n", i);
        exit(1);
      }
      if (ReadingObjectKey == state) {
        // {}
        //  ^
        // this object has no key and no value
        current_value->obj_pairs[current_value->len].key.size = 0;
        state = FinishedReadingObject;
        continue;
      }
      if (ReadingObjectValueInt == state) {
        long int value = flush_stack_buffer_to_int(&scratch);
        current_value->obj_pairs[current_value->len].value.type =
            JSON_value_type_Int;
        current_value->obj_pairs[current_value->len].value.int_val = value;
        current_value->len++;
        state = FinishedReadingObject;
        continue;
      }
      if (ReadingObjectValueFloat == state) {
        float value = flush_stack_buffer_to_float(&scratch);
        current_value->obj_pairs[current_value->len].value.type =
            JSON_value_type_Float;
        current_value->obj_pairs[current_value->len].value.float_val = value;
        current_value->len++;
        state = FinishedReadingObject;
        continue;
      }
      break;
    case '[':
      push_stack_buffer(c, &state_stack);
      if (state == StartState) {
        json.value.type = JSON_value_type_Array;
        json.value.arr_values =
            arena_push(json.arena, sizeof(JsonValue) * JSON_ARRAY_INITIAL_LEN)
                .val.res;
        json.value.capacity = JSON_ARRAY_INITIAL_LEN;
        json.value.len = 0;
        state = ReadingArrayElement;
        continue;
      }
      break;
    case ']':
      if ('[' != pop_stack_buffr(&state_stack)) {
        puts("unmatched brackes");
        exit(1);
      }
      if (ReadingArrayElement == state) {
        // this array has no values
        state = FinishedReadingArray;
        continue;
      }
      if (ReadingArrayElementInt == state) {
        state = FinishedReadingArray;
        if (json.value.len + 1 >= json.value.capacity) {
          puts("Array too long. TODO handle");
        }
        json.value.arr_values[json.value.len] = (JsonValue){0};
        json.value.arr_values[json.value.len].type = JSON_value_type_Int;
        long int value = flush_stack_buffer_to_int(&scratch);
        json.value.arr_values[json.value.len].int_val = value;
        json.value.len++;
        break;
      }
    case '"':
      if (state == ReadingObjectKeyString) {
        current_value->obj_pairs[current_value->len].key.memsize = scratch.len;
        current_value->obj_pairs[current_value->len].key.size = scratch.len;
        // const char* expected = "{\"key\":12345}";
        // if (!memcmp(expected, json_str, strlen(expected))) {
        // }
        char *dest = flush_scratch_buffer_string_to_arena(&scratch, json.arena);
        current_value->obj_pairs[current_value->len].key.str = dest;
        state = FinishedReadingObjectKey;
        continue;
      }
      if (state == ReadingObjectKey || state == FinishedReadingObjectValue) {
        state = ReadingObjectKeyString;
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      exit(1);
      break;
    case ',':
      if (ReadingArrayElementInt == state) {
        json.value.arr_values[json.value.len] = (JsonValue){0};
        json.value.arr_values[json.value.len].type = JSON_value_type_Int;
        long int value = flush_stack_buffer_to_int(&scratch);
        json.value.arr_values[json.value.len].int_val = value;
        json.value.len++;
        continue;
      }
      if (ReadingObjectValueInt == state) {
        long int value = flush_stack_buffer_to_int(&scratch);
        current_value->obj_pairs[current_value->len].value.type =
            JSON_value_type_Int;
        current_value->obj_pairs[current_value->len].value.int_val = value;
        current_value->len++;
        state = FinishedReadingObjectValue;
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      exit(1);
      break;
    case ':':
      if (FinishedReadingObjectKey == state) {
        state = ReadyToReadValue;
      } else {
        puts("unhandled");
        exit(1);
      }
      continue;
      break;
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
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      if (ReadingObjectValueInt == state && c == '.') {
        state = ReadingObjectValueFloat;
        add_char_to_stack_buffer(&scratch, c);
        continue;
      }
      printf("Error: unexpected character %c at index %zu", c, i);
      exit(1);
      break;
    }
    }
  }

  if (state_stack.len > 0) {
    puts("Unclosed characters in JSON");
    exit(1);
  }

  return json;
}

void str_push(String *s, char c) {
  if (s->size + 1 >= s->memsize) {
    puts("TODO: resize here str_push");
  }

  s->str[s->size] = c;
  s->size += 1;
}

void copy_int_to_string(String *s, long int v) {
  char buffr[STACK_BUFFER_LEN] = {0};
  snprintf(buffr, STACK_BUFFER_LEN, "%ld", v);
  size_t str_buffr_size = strlen(buffr);
  if (s->size + str_buffr_size > s->memsize) {
    puts("TODO: resize here int to string");
  }
  memcpy(&s->str[s->size], buffr, str_buffr_size);
  s->size += str_buffr_size;
}

void copy_float_to_string(String *s, float f) {
  char buffr[STACK_BUFFER_LEN] = {0};
  snprintf(buffr, STACK_BUFFER_LEN, "%f", f);
  size_t str_buffr_size = strlen(buffr);
  if (s->size + str_buffr_size > s->memsize) {
    puts("TODO: resize here float to string");
  }
  memcpy(&s->str[s->size], buffr, str_buffr_size);
  s->size += str_buffr_size;
}

void copy_key_to_string(String *s, String *key) {
  // + 2 for the characters `"":`
  if (s->size + key->size + 2 >= s->memsize) {
    puts("TODO: resize here key to string");
  }
  str_push(s, '"');
  memcpy(&s->str[s->size], key->str, key->size);
  s->size += key->size;
  str_push(s, '"');
}

void stringify_json_value(String *s, JsonValue *current_value) {
  StackBuffer state_stack = {0};
  switch (current_value->type) {
  case JSON_value_type_Object: {
    str_push(s, '{');
    push_stack_buffer('}', &state_stack);

    for (u32 i = 0; i < current_value->len; i++) {
      if (i > 0) {
        str_push(s, ',');
      }

      if (current_value->obj_pairs[0].key.size == 0) {
        str_push(s, pop_stack_buffr(&state_stack));
        continue;
      }

      copy_key_to_string(s, &(current_value->obj_pairs[i].key));
      str_push(s, ':');
      if (JSON_value_type_Int == current_value->obj_pairs[i].value.type) {
        copy_int_to_string(s, current_value->obj_pairs[i].value.int_val);
      }
      if (JSON_value_type_Float == current_value->obj_pairs[i].value.type) {
        copy_float_to_string(s, current_value->obj_pairs[i].value.float_val);
      }
      if (JSON_value_type_Object == current_value->obj_pairs[i].value.type) {
        stringify_json_value(s, &current_value->obj_pairs[i].value);
      }
    }
    break;
  };
  case JSON_value_type_Array: {
    str_push(s, '[');
    push_stack_buffer(']', &state_stack);
    for (size_t i = 0; i < current_value->len; i++) {
      JsonValue val = current_value->arr_values[i];
      if (JSON_value_type_Int == val.type) {
        copy_int_to_string(s, val.int_val);
        if (current_value->len > i + 1) {
          if (s->size + 1 > s->memsize) {
            puts("TODO: resize here array");
          }
          str_push(s, ',');
        }
      }
    }
    break;
  }
  case JSON_value_type_Float:
  case JSON_value_type_Int:
  case JSON_value_type_Boolean:
  case JSON_value_type_String:
    puts("This is not a valid json type at the start");
    exit(1);
  }

  while (state_stack.len > 0) {
    str_push(s, pop_stack_buffr(&state_stack));
  }

}

String stringify(Json json) {
  String s = {
      .size = 0,
      .memsize = STRINGIFY_INITIAL_LEN,
      .str = calloc(STRINGIFY_INITIAL_LEN, sizeof(char)),
  };
  stringify_json_value(&s, &json.value);
  return s;
}
