#include "../types/types.h"
#include "../types/strings.h"
#include <stdio.h>
#include "../data_structures/string_list.h"
#include "../memory/arenas.h"
#include "pdf.h"


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

StrResult writePDF(Arena* return_arena, PDF pdf) {
  StrResult s = {0};
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
  Arena* scratch_arena = arena_result.arena;
  StringList* strings = push_back_str(scratch_arena, NULL, cstr_to_str_arena_unsafe(scratch_arena, "\%PDF-").str);
  push_back_str(scratch_arena, strings, pdf.magic_number);
  push_back_str(scratch_arena, strings, cstr_to_str_arena_unsafe(scratch_arena, "\n").str);
  // TODO: handle results
  // push_back_str(arena, strings, print_cross_reference_table(pdf).str);

  /*
  // pdf parsing goes here

  push_str(strings, print_trailer());
  push_str(strings, cstr_to_str_unsafe("startxref"));
  push_str(strings, i64_to_str(get_cross_reference_table_offset()));
  */
  push_back_str(scratch_arena, strings, cstr_to_str_arena_unsafe(scratch_arena, "\%\%EOF").str);
  s.str = concat_strs(return_arena, strings);
  arena_free(scratch_arena);
  s.status = SUCCESS;
  return s;
}

u8 pdf_equal(PDF pdf1, PDF pdf2) {
  return 0;
}

StrResult print_cross_reference_table(Arena* arena, PDF pdf) {
  StrResult s = {0};


  s.status = SUCCESS;
  return s;
}
