#include "./string_list.h"

StringList* push_str(Arena* arena, StringList* head, String new_val) {
  if (NULL == head) {
    // TODO: check return values
    head = (StringList*)arena_push(arena, sizeof(StringList)).val.res;
    return head;
  }
  // TODO: the rest of the function
  return head;
}


