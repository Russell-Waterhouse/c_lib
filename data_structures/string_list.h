#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "../types/types.h"
#include "../types/strings.h"
#include "../memory/arenas.h"

typedef struct StringListNode {
    String data;
    struct StringListNode* next;
} StringList;

StringList* push_back_str(Arena* arena, StringList* head, String new_val);
String concat_strs(StringList* head);
void print_strings(StringList* head);

#endif
