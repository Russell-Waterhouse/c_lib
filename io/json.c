#include "json.h"


// dogwater code but I'm sketching on mobile.
// will clean up on desktop later. 

typedef enum  {
  new,
  isReadingObjectKey;
  isReadingObjectKeyString;
  isFinishedReadingObjectKey;
  isReadingIntegerValue;
  isReadingFloatingValue;
  isReadyToReadValue;
  isEscapedInKeyString;
  isEscapedInValueString;
  
} ParserStateMachineState;

Json parse(char* json_str, u32 json_str_len) {
  Json json = {0};
  json.arena = arena_create(KiB(256)).arena;
  char buffr[256] ={0};// todo: look up max number of characters allowed in a number in JSON
  char state_stack[256];
  ParserStateMachineState state = ;
  for(u32 i = 0; i < json_str_len; i++) {
    char c = json_str[i];
    if (is_whitespace(c)) {
      next;
    }
    switch (c) {
      case '{':
        push(state_stack, c);
        if (state==isReadingObjectKeyString || state==isReadingValueKeyString)
          push(buffr, c);
        
        if(state ==new || state == isReadyToReadValue) {
            state = isReadingObjectKey;
            continue;
        }
        printf("Error: unexpected character %c at index %lu", c, i);
        exit(1);
        break;
      case '}':
        char popped_state = pop(stack_state)
        if (popped_state != '{') {
          printf("Error: unexpected character %c at index %lu", c, i);
          exit(1);
        }
        break;
      case '[':
        break;
      case ']':
        break;
      case '"':
        if (state==isReadingObjectKeyString) {
          flush_buffr_to_json_object_key(json, buffr);
          continue;
        }
        if (state == isReadingObjectKey) {
          state = isReadingObjectKeyString;
          continue;
        }
        printf("Error: unexpected character %c at index %lu", c, i);
        exit(1);
        break;
      case ',':
        break;
      case ':':
        state = isReadyToReadValue;
        continue;
        break;
      default:
        // all other characters
        if (isReadingObjectKeyString ||
           isReadingInt ||
           isReadingFloat ||
           isReadingObjectValueString) {
          push(buffr, c);
          continue;
        }
        printf("Error: unexpected character %c at index %lu", c, i);
        exit(1);
        
        break;
    }
  }

  return json;
}
