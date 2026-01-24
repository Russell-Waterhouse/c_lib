#include "../types/types.h"
#include "../types/strings.h"
#include <string.h>
#include "dictionary.h"


/* TODO: implement */
Dictionary* dict_upsert(Arena* arena, Dictionary* dict, String key, String val) {
  if (NULL == dict) {
    // TODO: Check return values;
    dict = (Dictionary*)arena_push(arena, sizeof(Dictionary)).val.res;
    dict -> next = NULL;
    dict -> key = key;
    dict -> val = val;
    return dict;
  }

  Dictionary* curr = dict;
  Dictionary* next;
  while (curr -> next != NULL) {
    next = curr -> next;
    size_t smaller_str_len = curr->key.size < key.size ? curr->key.size : key.size;
    if (
        0 == strncmp(curr->key.str, key.str, smaller_str_len)
    ) {
      curr -> val = val;
      return dict;
    }
    if (
        0 < strncmp(curr->key.str, key.str, smaller_str_len) &&
        0 > strncmp(next->key.str, key.str, smaller_str_len)
    ) {
      curr -> val = val;
      break;
    }
  }

  Dictionary* tmp = curr -> next;
  curr -> next = (Dictionary*)arena_push(arena, sizeof(Dictionary)).val.res;
  curr -> next -> next = tmp;
  curr -> next -> key = key;
  curr -> next  -> val = val;

  return dict;
}
/* TODO: implement */
StrResult dict_get(Dictionary* dict, String key) {
  StrResult s = {0};
  if (NULL == dict) {
    s.status = FAIL;
    s.err.code = ERR_INVALID_ARG;
    s.err.msg = "Cannot GET from NULL dictionary";
    return s;
  }

  Dictionary* curr = dict;
  while (curr != NULL) {
    size_t smaller_str_len = curr->key.size < key.size ? curr->key.size : key.size;
    if (
      0 == strncmp(curr->key.str, key.str, smaller_str_len)
    ) {
      s.str = curr -> val;
      s.status = SUCCESS;
      return s;
    }
    if (
      0 > strncmp(curr->key.str, key.str, smaller_str_len)
    ) {
      // We've passed where it would be, return fail.
      s.status = FAIL;
      return s;
    }
  }
  s.status = FAIL;
  return s;
}

/* TODO: implement */
Result dict_delete(Dictionary* dict, String key) {
  return FAIL;
}

