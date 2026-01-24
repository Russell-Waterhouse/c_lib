#include "../data_structures/dictionary.h"
#include "stdio.h"
#include "../core/pretty_print.h"
#include "../types/types.h"

Result test_upsert() {
  Arena* arena = arena_create(MiB(1)).arena;
  String s1 = cstr_to_str_arena_unsafe(arena, "A").str;
  String s2 = cstr_to_str_arena_unsafe(arena, "Z").str;
  String s3 = cstr_to_str_arena_unsafe(arena, "A").str;
  Dictionary* dict = dict_upsert(arena, NULL, s1, s2);
  StrResult res = dict_get(dict, s3);
  if (
      res.status == SUCCESS &&
      str_equal(s2, res.str)
  ){
    arena_free(arena);
    return SUCCESS;
  }

  arena_free(arena);
  return FAIL;
}

void test_dictionary() {
  puts("Starting dictionary tests.");
  if (
      test_upsert() == SUCCESS
  ) {
    print_green("dictionary tests completed successfully");
    return;
  }

  print_red("There were dictionary test failures");
  return;
}
