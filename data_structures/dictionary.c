#include "../types/types.h"
#include "../types/strings.h"
#include "../memory/arenas.h"
#include <string.h>
#include "dictionary.h"


DictionaryResult* dict_upsert(Arena* arena, Dictionary* dict, String key, String val) {
  DictionaryResult* res = {0};
  /*
  if (NULL == dict) {
    // TODO: this is programmingon my phone, doesn't compile.
    Dictionary* allocResult = (Dictionary*)arena_push(arena, sizeof(Dictionary));
    if (dict.status == FAIL) {
      res.status = FAIL;
      res.err = allocResult.val.err;
      return res;
    }
    dict = allocResult.val;
    dict -> next = NULL;
    dict -> key = key;
    dict -> val = val;
    res.status = SUCCESS;
    res.dict = dict;
    return res;
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
      res.res = SUCCESS;
      res.dict = dict;
      return res;
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
  */
  res->res.status = FAIL; // TODO: change to success when this actually works.
  // res->dict = dict;
  return res;
}

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

DictionaryResult dict_delete(Dictionary* dict, String key) {
  DictionaryResult s = { 0 };
  if (NULL == dict) {
    s.res.status = FAIL;
    s.err.code = ERR_INVALID_ARG;
    s.err.msg = "Cannot GET from NULL dictionary";
    return s;
  }

  // TODO implement
  return s;
}

