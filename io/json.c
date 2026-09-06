#include "json.h"
#include "../core/debugging.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define JSON_ARRAY_INITIAL_LEN 256
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
  FinhedReadingObjectKey,
  ReadingObjectValueInt,
  ReadingObjectValueFloat,
  ReadyToReadValue,
  EscapedInKeyString,
  EscapedInValueString,
  ReadingValueKeyString,
  FinishedReadingObject,
  FinishedReadingObjectKey,
  ReadingArrayElement,
  ReadingArrayElementInt,
  FinishedReadingArray,
} ParserStateMachineState;

Json parse(char *json_str, size_t json_str_len) {
  Json json = {0};
  json.arena = arena_create(KiB(256)).arena;
  // todo: look up max number of characters allowed in a number in JSON
  StackBuffer scratch = {0};
  StackBuffer state_stack = {0};
  ParserStateMachineState state = StartState;
  for (u32 i = 0; i < json_str_len; i++) {
    char c = json_str[i];
    if (isspace(c)) {
      continue;
    }
    switch (c) {
    case '{':
      push_stack_buffer(c, &state_stack);
      if (StartState == state) {
        json.start = JsonStartObject;
        state = ReadingObjectKey;
        continue;
      }
      break;
    case '}':
      if ('{' != pop_stack_buffr(&state_stack)) {
        puts("unmatched curly's");
        exit(1);
      }
      if (ReadingObjectKey == state) {
        // this object has no key and no value
        state = FinishedReadingObject;
        continue;
      }
      if (ReadingObjectValueInt == state) {
        long int value = flush_stack_buffer_to_int(&scratch);
        json.obj.value.type = JSON_value_type_Int;
        json.obj.value.int_val = value;
        state = FinishedReadingObject;
        continue;
      }
      if (ReadingObjectValueFloat == state) {
        float value = flush_stack_buffer_to_float(&scratch);
        json.obj.value.type = JSON_value_type_Float;
        json.obj.value.float_val = value;
        state = FinishedReadingObject;
        continue;
      }
      break;
    case '[':
      push_stack_buffer(c, &state_stack);
      if (state == StartState) {
        json.start = JsonStartArray;
        json.arr.values =
            arena_push(json.arena, sizeof(JsonValue) * JSON_ARRAY_INITIAL_LEN)
                .val.res;
        json.arr.capacity = JSON_ARRAY_INITIAL_LEN;
        json.arr.len = 0;
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
        if (json.arr.len + 1 >= json.arr.capacity) {
          puts("Array too long. TODO handle");
        }
        json.arr.values[json.arr.len] = (JsonValue){0};
        json.arr.values[json.arr.len].type = JSON_value_type_Int;
        long int value = flush_stack_buffer_to_int(&scratch);
        json.arr.values[json.arr.len].int_val = value;
        json.arr.len++;
        break;
      }
    case '"':
      if (state == ReadingObjectKeyString) {
        json.obj.key.memsize = scratch.len;
        json.obj.key.size = scratch.len;
        char *dest = flush_scratch_buffer_string_to_arena(&scratch, json.arena);
        json.obj.key.str = dest;
        state = FinishedReadingObjectKey;
        continue;
      }
      if (state == ReadingObjectKey) {
        state = ReadingObjectKeyString;
        continue;
      }
      printf("Error: unexpected character %c at index %u", c, i);
      exit(1);
      break;
    case ',':
      if (ReadingArrayElementInt == state) {
        json.arr.values[json.arr.len] = (JsonValue){0};
        json.arr.values[json.arr.len].type = JSON_value_type_Int;
        long int value = flush_stack_buffer_to_int(&scratch);
        json.arr.values[json.arr.len].int_val = value;
        json.arr.len++;
      }
      break;
    case ':':
      if (FinishedReadingObjectKey == state) {
        state = ReadyToReadValue;
      } else {
        puts("unhandled");
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
        printf("Error: unexpected character %c at index %u", c, i);
        exit(1);
        break;
      }
    }
    }
  }

  if (state_stack.len > 0) {
    puts("Unclosed characters in JSON");
    exit(1);
  }

  return json;
}

String stringify(Json json) {
  String s = {
      .size = 0,
      .memsize = STRINGIFY_INITIAL_LEN,
      .str = calloc(STRINGIFY_INITIAL_LEN, sizeof(char)),
  };
  switch (json.start) {
  case JsonStartObject: {
    s.str[s.size] = '{';
    s.size += 1;
    if (0 == json.obj.key.size) {
      s.str[s.size] = '}';
      s.size += 1;
      return s;
    }
    // + 2 for the characters `"":`
    if (s.size + json.obj.key.size + 2 > s.memsize) {
      puts("TODO: resize here");
    }
    s.str[s.size] = '"';
    s.size += 1;
    memcpy(&s.str[s.size], json.obj.key.str, json.obj.key.size);
    s.size += json.obj.key.size;
    s.str[s.size] = '"';
    s.size += 1;
    s.str[s.size] = ':';
    s.size += 1;
    if (JSON_value_type_Int == json.obj.value.type) {
      char buffr[STACK_BUFFER_LEN] = {0};
      snprintf(buffr, STACK_BUFFER_LEN, "%ld", json.obj.value.int_val);
      size_t str_buffr_size = strlen(buffr);
      if (s.size + str_buffr_size > s.memsize) {
        puts("TODO: resize here");
      }
      memcpy(&s.str[s.size], buffr, str_buffr_size);
      s.size += str_buffr_size;
      s.str[s.size] = '}';
      s.size += 1;
    }
    if (JSON_value_type_Float == json.obj.value.type) {
      char buffr[STACK_BUFFER_LEN] = {0};
      snprintf(buffr, STACK_BUFFER_LEN, "%f", json.obj.value.float_val);
      size_t str_buffr_size = strlen(buffr);
      if (s.size + str_buffr_size > s.memsize) {
        puts("TODO: resize here");
      }
      memcpy(&s.str[s.size], buffr, str_buffr_size);
      s.size += str_buffr_size;
      s.str[s.size] = '}';
      s.size += 1;
    }

    break;
  };
  case JsonStartArray: {
    s.str[s.size] = '[';
    s.size += 1;
    for (size_t i = 0; i < json.arr.len; i++) {
      JsonValue val = json.arr.values[i];
      if (JSON_value_type_Int == val.type) {
        char buffr[STACK_BUFFER_LEN] = {0};
        snprintf(buffr, STACK_BUFFER_LEN, "%ld", val.int_val);
        size_t str_buffr_size = strlen(buffr);
        if (s.size + str_buffr_size > s.memsize) {
          puts("TODO: resize here");
        }
        memcpy(&s.str[s.size], buffr, str_buffr_size);
        s.size += str_buffr_size;
        if (json.arr.len > i + 1) {
          if (s.size + 1 > s.memsize) {
            puts("TODO: resize here");
          }
          s.str[s.size] = ',';
          s.size += 1;
        }
      }
    }
    if (s.size + 1 > s.memsize) {
      puts("TODO: resize here");
    }
    s.str[s.size] = ']';
    s.size += 1;
    return s;
    break;
  }
  }

  return s;
}
