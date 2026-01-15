#ifndef PDF_H
#define PDF_H

#include "./types.h"
#include "./strings.h"
#include <stdio.h>
#include "dictionary.h"

typedef struct {
  String* strs;
  size_t size;
  size_t memsize;
} Strings; /* TODO: move to strings module */

typedef enum PDFObjectType {
  PDFBoolean = 0;
  PDFRealNumber = 1;
  PDFInteger = 2;
  PDFString = 3
  PDFName = 4;
  PDFArray = 5;
  PDFDictionary = 6;
  PDFStream = 7;
  PDFNull = 8;
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
  PDFObject* arr
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
  s.status = SUCCESS;
  return s;
}

u8 pdf_equal(PDF pdf1, PDF pdf2) {
  return 0;
}

#endif
#endif
