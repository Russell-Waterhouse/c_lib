#include "./string_list.h"

StringList* push_back_str(Arena* arena, StringList* head, String new_val) {
  if (NULL == head) {
    // TODO: check return values
    head = (StringList*)arena_push(arena, sizeof(StringList)).val.res;
    head -> next = NULL;
    head -> data = new_val;
    return head;
  }

  StringList* cur = head;
  while(cur->next != NULL) {
    cur = cur->next;
  }

  // TODO: check return values
  cur -> next = (StringList*)arena_push(arena, sizeof(StringList)).val.res;

  cur -> next -> next = NULL;
  cur -> data = new_val;
  return head;
}


String concat_strs(StringList* head) {
  size_t full_len = 0;
  StringList* cur = head;
  while (NULL != cur) {
    full_len += cur->data.size;
    cur = cur->next;
  }
  String res = {
    .size = full_len,
    .memsize = (sizeof(char) * full_len) + 1, // + 1 for \0
    .str = NULL
  };
  res.str = malloc(res.memsize);
  // TODO: check not null
  cur = head;
  size_t i = 0;
  size_t j = 0;

  while (NULL != cur) {
    for (j = 0; j < cur->data.size; j++) {
      res.str[i] = cur->data.str[j];
      i++;
    }
    cur = cur -> next;
  }
  res.str[i] = '\0';
  return res;
}
