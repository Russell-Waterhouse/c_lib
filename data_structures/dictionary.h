#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "./strings.h"

/*
 * String key-value pairs
 */
typedef struct {

} Dictionary;


Result dict_upsert(Dictionary* dict, String key, String val);
String dict_get(Dictionary* dict, String key);
Result dict_delete(Dictionary* dict, String key);

#ifdef DICTIONARY_IMPLEMENTATION

/* TODO: implement */
Result dict_upsert(Dictionary* dict, String key, String val) {
  return FAIL;
}
/* TODO: implement */
String dict_get(Dictionary* dict, String key) {
  String s = {0};
  return s;
}

/* TODO: implement */
Result dict_delete(Dictionary* dict, String key) {
  return FAIL;
}

#endif
#endif
