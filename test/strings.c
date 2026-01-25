#include <stdio.h>
#include <string.h>
#include "../types/types.h"
#include "../types/strings.h"
#include "../core/pretty_print.h"

Result test_cstr_to_str() {
  char s1[] = "";
  char s2[] = "This is a string";
  char s3[] = "This is another string";
  Arena* arena = arena_create(MiB(1)).arena;
  StrResult s = cstr_to_str(arena, s1, strlen(s1));
  if (s.status != SUCCESS || s.str.size != 0 || s.str.memsize != 0) {
    printf("case 1 failed; size: %ld; memsize: %ld;\n", s.str.size, s.str.memsize);
    arena_free(arena);
    return FAIL;
  }

  s = cstr_to_str(arena, s2, strlen(s2));
  if (s.status != SUCCESS || s.str.size != 16 || s.str.memsize != 16 || !strcmp(s1, s.str.str)) {
    printf("case 2 failed; size: %ld, memsize: %ld, string: %s\n",s.str.size, s.str.memsize, s.str.str);
    arena_free(arena);
    return FAIL;
  }

  s = cstr_to_str(arena, s3, strlen(s3));
  if (s.status != SUCCESS || s.str.size != 22 || s.str.memsize != 22 || !strcmp(s2, s.str.str)) {
    printf("case 3 failed\n");
    arena_free(arena);
    return FAIL;
  }

  arena_free(arena);
  return SUCCESS;
}

/* TODO */
Result test_equal() {
  return SUCCESS;
}

Result test_split() {
  Arena* arena = arena_create(KiB(10)).arena;
  String s1 = cstr_to_str(arena, "2000-01-99", strlen("2000-01-99")).str;
  String e1 = cstr_to_str(arena, "2000", strlen("2000")).str;
  String e2 = cstr_to_str(arena,"01", strlen("01")).str;
  String e3 = cstr_to_str(arena, "99", strlen("99")).str;
  SplitResultOption split_strings = split_str(arena, s1, '-');
  String r1 = split_strings.strs.arr[0];
  String r2 = split_strings.strs.arr[1];
  String r3 = split_strings.strs.arr[2];

  if (
    split_strings.status == SUCCESS &&
    split_strings.strs.size == 3 &&
    str_equal(e1, r1) &&
    str_equal(e2, r2) &&
    str_equal(e3, r3)
  ) {
    arena_free(arena);
    free_dyn_str_arr(split_strings.strs);
    return SUCCESS;
  }
  arena_free(arena);
  free_dyn_str_arr(split_strings.strs);
  return FAIL;
}

Result test_make_kmp_fail_table() {
  Arena* arena = arena_create(KiB(10)).arena;
  char* cstr = "ABCDABD";
  String w = cstr_to_str(arena, cstr, strlen(cstr)).str;
  i64DynArr expected;
  i64DynArr actual;

  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  expected = i64_insert_back_or_die(expected, 0);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  expected = i64_insert_back_or_die(expected, 2);
  expected = i64_insert_back_or_die(expected, 0);
  actual = make_kmp_fail_table(w);

  if (!i64_equal(expected, actual)) {
    puts("make_kmp_fail_table failed example from wikipedia");
    i64_free(expected);
    i64_free(actual);
    arena_free(arena);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);

  cstr = "ABACABABA";
  w = cstr_to_str(arena, cstr, strlen(cstr)).str;
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 3);
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, -3);
  actual = make_kmp_fail_table(w);

  if (!i64_equal(expected, actual)) {
    puts("make_kmp_fail_table failed example 2 from wikipedia");
    i64_free(expected);
    i64_free(actual);
    arena_free(arena);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);


  cstr = "AB";
  w = cstr_to_str(arena, cstr, strlen(cstr)).str;
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  actual = make_kmp_fail_table(w);

  if (!i64_equal(expected, actual)) {
    puts("make_kmp_fail_table failed my example");
    i64_free(expected);
    i64_free(actual);
    arena_free(arena);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);
  arena_free(arena);

  return SUCCESS;
}

Result test_find_all() {
  Arena* arena = arena_create(KiB(10)).arena;
  char* cstr_s = "";
  char* cstr_w = "";
  String s = cstr_to_str(arena, "", 0).str;
  String w = cstr_to_str(arena, "", 0).str;
  SizeTDynArr res = find_all(s, w);
  if (0 != res.size) {
    size_t_free(res);
    arena_free(arena);
    return FAIL;
  }
  size_t_free(res);

  cstr_s = "A";
  cstr_w = "AB";
  s = cstr_to_str(arena, cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(arena, cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (0 != res.size) {
    size_t_free(res);
    arena_free(arena);
    return FAIL;
  }
  size_t_free(res);

  cstr_s = "AB";
  cstr_w = "AB";
  s = cstr_to_str(arena, cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(arena, cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (1 != res.size || 0 != res.arr[0]) {
    size_t_free(res);
    arena_free(arena);
    puts("Failed to find single match when strings are equal");
    return FAIL;
  }
  size_t_free(res);

  cstr_s = "ABABABABAB";
  cstr_w = "AB";
  s = cstr_to_str(arena, cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(arena, cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (
      5 != res.size ||
      0 != res.arr[0] ||
      2 != res.arr[1] ||
      4 != res.arr[2] ||
      6 != res.arr[3] ||
      8 != res.arr[4]
  ) {
    size_t_free(res);
    arena_free(arena);
    puts("Failed to find multiple match when strings are equal");
    return FAIL;
  }
  size_t_free(res);
  arena_free(arena);
  return SUCCESS;
}

void test_strings() {
  puts("Starting string tests.");
  if (
      test_cstr_to_str() == SUCCESS &&
      test_equal() == SUCCESS &&
      test_split() == SUCCESS &&
      test_find_all() == SUCCESS
  ) {
    print_green("Tests completed successfully!");
    return;
  }


  print_red("There were test failures.");
  return;
}
