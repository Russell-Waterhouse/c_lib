#include <stdio.h>
#include <stdlib.h>
#include "../data_structures/linked_list.h"
#include "../types/types.h"
#include "../core/pretty_print.h"

Result test_insert() {
  u64 i;
  Node* head = GENERIC_TYPE_insert_front(NULL, 0);
  for(i = 0; i < 8; i++) {
    head = GENERIC_TYPE_insert_front(head, i);
  }
  Node* curr = head;
  u64 max = 0;
  --max;
  for(i = 7; i != max; i--) {
    if (curr -> value != i) {
      printf("TEST FAIL: %lu != %lu\n", curr -> value, i);
      GENERIC_TYPE_free_list(head);
      return FAIL;
    }
    curr = curr -> next;
  }

  GENERIC_TYPE_free_list(head);
  puts("Insertion test completed.");
  return SUCCESS;
}

Result test_delete() {
  return SUCCESS;
}

void test_ll() {
  puts("Starting Linked Lists tests.");
  if (test_insert() == SUCCESS && test_delete() == SUCCESS) {
    print_green("Tests completed Linked Lists successfully!");
    return;
  }
  print_red("There were test failures.");
  return;
}

