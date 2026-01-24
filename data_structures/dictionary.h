#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "../types/strings.h"
#include "../types/types.h"

typedef struct {
  String key;
  String val;
} DictEntry;

typedef struct {
  size_t size;
  size_t memsize;
  DictEntry* entries;
} Dictionary;

Result dict_upsert(Dictionary* dict, String key, String val);
String dict_get(Dictionary* dict, String key);
Result dict_delete(Dictionary* dict, String key);


#endif
