#ifndef PDF_H
#define PDF_H

#include "../types/types.h"
#include "./strings.h"
#include <stdio.h>
#include "../data_structures/dictionary.h"
#include "../memory/arenas.h"

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
  u8 boolean;
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
StrResult print_cross_reference_table(Arena* arena, PDF pdf);
u8 pdf_equal(PDF pdf1, PDF pdf2);

#endif
