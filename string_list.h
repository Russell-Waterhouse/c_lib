#ifndef STRING_LIST_H

#include "./types.h"
#include "./arenas.h"
#include "./strings.h"

typedef struct StringListNode {
    int data;
    struct StringListNode *next;
} StringList;

#ifdef STRING_LIST_IMPLEMENTATION
StringList* push_str(Arena* arena, StringList* head, String new_val) {
  if (NULL == head) {
    // TODO: check return values
    head = (StringList*)arena_push(arena, sizeof(StringList)).val.res;
    return head;
  }
  // TODO: the rest of the function
  return head;
}


#endif
#endif
