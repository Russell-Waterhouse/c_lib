#include "json.h"
#include "../core/debugging.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

typedef enum {
  StartState,
  ReadingObjectKey,
  ReadingObjectKeyString,
  FinhedReadingObjectKey,
  ReadingIntegerValue,
  ReadingFloatingValue,
  ReadyToReadValue,
  EscapedInKeyString,
  EscapedInValueString,
  ReadingValueKeyString,
  FinishedReadingObject,
  FinishedReadingObjectKey,
} ParserStateMachineState;

void handle_closing_brace_reading_int(ParserStateMachineState *state,
                                      StackBuffer *scratch, Json *json) {
  if (scratch->len + 1 >= STACK_BUFFER_LEN) {
    puts("integer too long");
    exit(1);
  }
  scratch->buffr[scratch->len] = '\0';
  long int value = strtol(scratch->buffr, NULL, BASE_TEN);
  // TODO: there's more to strtol for error handling
  json->obj.value.type = JSON_value_type_Int;
  json->obj.value.int_val = value;
  *state = FinishedReadingObject;
}

void handle_closing_brace_reading_float(ParserStateMachineState *state,
                                        StackBuffer *scratch, Json *json) {
  if (scratch->len + 1 >= STACK_BUFFER_LEN) {
    puts("key too long");
    exit(1);
  }
  scratch->buffr[scratch->len] = '\0';
  float value = strtof(scratch->buffr, NULL);
  // TODO: there's more to strtof for error handling
  json->obj.value.type = JSON_value_type_Float;
  json->obj.value.float_val = value;
  *state = FinishedReadingObject;
}

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
      if (ReadingIntegerValue == state) {
        handle_closing_brace_reading_int(&state, &scratch, &json);
        continue;
      }
      if (ReadingFloatingValue == state) {
        handle_closing_brace_reading_float(&state, &scratch, &json);
        continue;
      }
      break;
    case '[':
      break;
    case ']':
      break;
    case '"':
      if (state == ReadingObjectKeyString) {
        char *dest = (char *)(arena_push(json.arena, scratch.len).val.res);
        memcpy(dest, scratch.buffr, scratch.len);
        state = FinishedReadingObjectKey;
        json.obj.key.str = dest;
        json.obj.key.memsize = scratch.len;
        json.obj.key.size = scratch.len;
        scratch.len = 0;
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
        state = ReadingIntegerValue;
      }
      if ((ReadingIntegerValue == state || ReadingFloatingValue == state) &&
          isdigit(c)) {
        if (scratch.len + 1 >= STACK_BUFFER_LEN) {
          puts("key too long");
          continue;
        }
        scratch.buffr[scratch.len] = c;
        scratch.len += 1;
        continue;
      }
      if (ReadingObjectKeyString == state) {
        if (scratch.len + 1 >= STACK_BUFFER_LEN) {
          puts("key too long");
          continue;
        }
        scratch.buffr[scratch.len] = c;
        scratch.len += 1;
        continue;
      }
      if (ReadingIntegerValue == state && c == '.') {
        state = ReadingFloatingValue;
        if (scratch.len + 1 >= STACK_BUFFER_LEN) {
          puts("key too long");
          continue;
        }
        scratch.buffr[scratch.len] = c;
        scratch.len += 1;
        continue;
        printf("Error: unexpected character %c at index %u", c, i);
        exit(1);
        break;
      }
    }
    }
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
    puts("TODO: handle case");
    break;
  }
  }

  return s;
}
