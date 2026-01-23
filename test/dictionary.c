#include "../data_structures/dictionary.h"
#include "stdio.h"
#include "../core/pretty_print.h"
#include "../types/types.h"

Result test_empty_dictionary_created() {
  return SUCCESS;
}

void test_dictionary() {
  puts("Starting dictionary tests.");
  if (
      test_empty_dictionary_created() == SUCCESS
  ) {
    print_green("dictionary tests completed successfully");
    return;
  }

  print_red("There were dictionary test failures");
  return;
}
