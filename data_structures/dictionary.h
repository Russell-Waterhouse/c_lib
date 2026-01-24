#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "../types/strings.h"
#include "../types/types.h"

typedef struct DictNode{
  String key;
  String val;
  struct DictNode* next;
} Dictionary;

Dictionary* dict_upsert(Arena* arena, Dictionary* dict, String key, String val);
StrResult dict_get(Dictionary* dict, String key);
Result dict_delete(Dictionary* dict, String key);


#endif
