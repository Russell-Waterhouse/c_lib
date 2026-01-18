#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "../types/types.h"
#include "../types/strings.h"
#include "../memory/arenas.h"

typedef struct StringListNode {
    int data;
    struct StringListNode *next;
} StringList;

StringList* push_str(Arena* arena, StringList* head, String new_val);

#endif
