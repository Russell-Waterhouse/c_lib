#include <stdio.h>
#include <string.h>
#include "../types.h"
#include "../strings.h"
#include "../pretty_print.h"

Result test_cstr_to_str() {
  char s1[] = "";
  char s2[] = "This is a string";
  char s3[] = "This is another string";
  StrResult s = cstr_to_str(s1, strlen(s1));
  if (s.status != SUCCESS || s.str.size != 0 || s.str.memsize != 0) {
    printf("case 1 failed; size: %ld; memsize: %ld;\n", s.str.size, s.str.memsize);
    return FAIL;
  }
  free_str(s.str);

  s = cstr_to_str(s2, strlen(s2));
  if (s.status != SUCCESS || s.str.size != 16 || s.str.memsize != 16 || !strcmp(s1, s.str.str)) {
    printf("case 2 failed; size: %ld, memsize: %ld, string: %s\n",s.str.size, s.str.memsize, s.str.str);
    return FAIL;
  }
  free_str(s.str);

  s = cstr_to_str(s3, strlen(s3));
  if (s.status != SUCCESS || s.str.size != 22 || s.str.memsize != 22 || !strcmp(s2, s.str.str)) {
    printf("case 3 failed\n");
    return FAIL;
  }
  free_str(s.str);

  return SUCCESS;
}

/* TODO */
Result test_equal() {
  return SUCCESS;
}

Result test_split() {
  String s1 = cstr_to_str("2000-01-99", strlen("2000-01-99")).str;
  String e1 = cstr_to_str("2000", strlen("2000")).str;
  String e2 = cstr_to_str("01", strlen("01")).str;
  String e3 = cstr_to_str("99", strlen("99")).str;
  SplitResultOption split_strings = split_str(s1, '-');
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
    return SUCCESS;
  }
  return FAIL;
}

Result test_make_kmp_fail_table() {
  char* cstr = "ABCDABD";
  String w = cstr_to_str(cstr, strlen(cstr)).str;
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
    free_str(w);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);
  free_str(w);

  cstr = "ABACABABA";
  w = cstr_to_str(cstr, strlen(cstr)).str;
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
    free_str(w);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);
  free_str(w);


  cstr = "AB";
  w = cstr_to_str(cstr, strlen(cstr)).str;
  expected = i64_insert_back_or_die(expected, -1);
  expected = i64_insert_back_or_die(expected, 0);
  actual = make_kmp_fail_table(w);

  if (!i64_equal(expected, actual)) {
    puts("make_kmp_fail_table failed my example");
    i64_free(expected);
    i64_free(actual);
    free_str(w);
    return FAIL;
  }
  i64_free(expected);
  i64_free(actual);
  free_str(w);

  return SUCCESS;
}

void free_structs(String s, String w, SizeTDynArr res) {
  free_str(s);
  free_str(w);
  size_t_free(res);
}

Result test_find_all() {
  char* cstr_s = "";
  char* cstr_w = "";
  String s = cstr_to_str("", 0).str;
  String w = cstr_to_str("", 0).str;
  SizeTDynArr res = find_all(s, w);
  if (0 != res.size) {
    free_structs(s, w, res);
    return FAIL;
  }
  free_structs(s, w, res);

  cstr_s = "A";
  cstr_w = "AB";
  s = cstr_to_str(cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (0 != res.size) {
    free_structs(s, w, res);
    return FAIL;
  }
  free_structs(s, w, res);

  cstr_s = "AB";
  cstr_w = "AB";
  s = cstr_to_str(cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (1 != res.size || 0 != res.arr[0]) {
    free_structs(s, w, res);
    puts("Failed to find single match when strings are equal");
    return FAIL;
  }
  free_structs(s, w, res);
  cstr_s = "ABABABABAB";
  cstr_w = "AB";
  s = cstr_to_str(cstr_s, strlen(cstr_s)).str;
  w = cstr_to_str(cstr_w, strlen(cstr_w)).str;
  res = find_all(s, w);
  if (
      5 != res.size ||
      0 != res.arr[0] ||
      2 != res.arr[1] ||
      4 != res.arr[2] ||
      6 != res.arr[3] ||
      8 != res.arr[4]
  ) {
    free_structs(s, w, res);
    puts("Failed to find multiple match when strings are equal");
    return FAIL;
  }
  free_structs(s, w, res);
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
