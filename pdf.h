#ifndef PDF_H
#define PDF_H

#include "./types.h"
#include "./strings.h"
#include <stdio.h>

typedef struct PDFNode {

} PDFNode;


typedef struct {
  String magic_number;

} PDF;

typedef struct {
  Result status;
  PDF val;
  Error err;
} PDFResult;


PDFResult parsePDF(FILE* file);
String writePDF(PDF pdf);
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

String writePDF(PDF pdf) {
  /* TODO */
  String s;
  s.size = 0;
  return s;
}

u8 pdf_equal(PDF pdf1, PDF pdf2) {
  return 0;
}

#endif
#endif
