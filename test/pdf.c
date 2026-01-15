#include "../pdf.h"
#include "stdio.h"
#include "../pretty_print.h"
#include "../types.h"
#include "../pdf.h"
#include <string.h>
#include "../strings.h"

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

Result test_write_minimal_input() {
  PDF pdf;
  pdf.magic_number = cstr_to_str("1.7", 3).str;
  String actual = writePDF(pdf);
  char* s = "%PDF-1.7\n%%EOF";
  String expected = cstr_to_str(s, strlen(s)).str;
  if (str_equal(expected, actual)) {
    free_str(pdf.magic_number);
    return SUCCESS;
  }

  free_str(pdf.magic_number);
  return FAIL;
}

void test_pdf() {
  puts("Starting PDF tests.");
  if (
      test_null_file_handle_fails() == SUCCESS &&
      test_write_minimal_input() == SUCCESS
  ) {
    print_green("PDF tests completed successfully");
    return;
  }

  print_red("There were PDF test failures");
  return;
}
