#include "../pdf.h"
#include "stdio.h"
#include "../pretty_print.h"
#include "../types.h"
#include "../pdf.h"
#include <string.h>

Result test_null_file_handle_fails() {
  PDFResult res = parsePDF(NULL);
  if (
      res.status == FAIL &&
      res.err.code == ERR_INVALID_ARG &&
      !strcmp(res.err.msg, "NULL file handle passed to parsePDF")
  ) {
    return SUCCESS;
  }
  return FAIL;
}

void test_pdf() {
  puts("Starting PDF tests.");
  if (
      test_null_file_handle_fails() == SUCCESS
  ) {
    print_green("PDF tests completed successfully");
    return;
  }

  print_red("There were PDF test failures");
  return;
}
