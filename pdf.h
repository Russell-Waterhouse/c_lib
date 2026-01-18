#ifndef PDF_H
#define PDF_H

#include "./types.h"
#include "./strings.h"
#include <stdio.h>
#include "dictionary.h"
#include "arenas.h"

typedef struct {
  String* strs;
  size_t size;
  size_t memsize;
} Strings; /* TODO: move to strings module */

typedef enum PDFObjectType {
  PDFBoolean = 0,
  PDFRealNumber = 1,
  PDFInteger = 2,
  PDFString = 3,
  PDFName = 4,
  PDFArray = 5,
  PDFDictionary = 6,
  PDFStream = 7,
  PDFNull = 8,
} PDFObjectType;

typedef struct {
  u8* bytes;
  size_t size;
  size_t memsize;
} Stream;

typedef union {
  u8 bool;
  double real_number;
  i64 integer;
  String string;
  String name;
  Strings array; /*TODO: should this be strings? */
  Dictionary dict;
  Stream stream;
} PDFObjecfVal;

typedef struct PDFObject {
  PDFObjectType type;
  PDFObjecfVal val;
} PDFObject;

typedef struct {
  PDFObject* arr;
  size_t size;
  size_t memsize;
} PDFObjects;

typedef struct {
  String magic_number;
  PDFObjects objects;
} PDF;

typedef struct {
  Result status;
  PDF val;
  Error err;
} PDFResult;


PDFResult parsePDF(FILE* file);
StrResult writePDF(PDF pdf);
u8 pdf_equal(PDF pdf1, PDF pdf2);

#ifdef PDF_IMPLEMENTATION

#include "./string_list.h"


PDFResult parsePDF(FILE* file) {
  PDFResult p;
  if (NULL == file) {
    p.status = FAIL;
    p.err.code = ERR_INVALID_ARG;
    p.err.msg = "NULL file handle passed to parsePDF";
    return p;
  }
  p.status = FAIL;
  return p;
}

StrResult writePDF(PDF pdf) {
  StrResult s;
  if (pdf.magic_number.size == 0 || NULL == pdf.magic_number.str) {
    s.status = FAIL;
    s.err.code = ERR_INVALID_ARG;
    s.err.msg = "Cannot write PDF with null magic number";
    return s;
  }
  /* TODO: the following is just how I want this to look,
  make it compile */
  ArenaResult arena_result = arena_create(MiB(1));
  if (arena_result.status != SUCCESS) {
    s.status = FAIL;
    s.err.code = arena_result.err.code;
    s.err.msg = arena_result.err.msg;
    return s;
  }
  Arena* arena = arena_result.arena;
  /*
  StringList* strings = NULL;
  strings = push_str(arena, strings, cstr_to_str_unsafe("%PDF-"));
  push_str(strings, magic_number);
  
  // pdf parsing goes here
  
  push_str(strings, print_cross_reference_table());
  push_str(strings, print_trailer());
  push_str(strings, cstr_to_str_unsafe("startxref"));
  push_str(strings, i64_to_str(get_cross_reference_table_offset()));
  push_str(strings, cstr_to_str_unsafe("%%EOF"));

  s.str = concat_strs(strings);
*/
  arena_free(arena);
  s.status = SUCCESS;
  return s;
}

u8 pdf_equal(PDF pdf1, PDF pdf2) {
  return 0;
}

#endif
#endif
