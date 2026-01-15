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
