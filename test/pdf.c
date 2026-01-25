#include "../io/pdf.h"
#include "stdio.h"
#include "../core/pretty_print.h"
#include "../types/types.h"
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

Result test_cross_reference_table() {
  // TODO: START HERE
  PDF pdf;
  Arena* arena = arena_create(MiB(1)).arena;
  pdf.magic_number = cstr_to_str_unsafe(arena, "1.7").str;
  String actual = print_cross_reference_table(arena, pdf).str;
  String expected = cstr_to_str_unsafe(arena, "").str;
  if (str_equal(expected, actual)) {
    arena_free(arena);
    return SUCCESS;
  }
  arena_free(arena);
  return SUCCESS;
}

Result test_write_minimal_input() {
  Arena* arena = arena_create(KiB(10)).arena;
  PDF pdf;
  pdf.magic_number = cstr_to_str_unsafe(arena, "1.7").str;
  StrResult res = writePDF(arena, pdf);
  String actual = res.str;
  String expected = cstr_to_str_unsafe(arena, "\%PDF-1.7\n%%EOF").str;
  pretty_print_string(expected);
  pretty_print_string(actual);
  if (str_equal(expected, actual)) {
    arena_free(arena);
    return SUCCESS;
  }

  arena_free(arena);
  return FAIL;
}

void test_pdf() {
  puts("Starting PDF tests.");
  if (
      test_null_file_handle_fails() == SUCCESS &&
      test_write_minimal_input() == SUCCESS &&
      test_cross_reference_table() == SUCCESS
  ) {
    print_green("PDF tests completed successfully");
    return;
  }

  print_red("There were PDF test failures");
  return;
}
