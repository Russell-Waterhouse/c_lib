#include "./unity.h"
#include "./strings.c"
#include "./linked_list.c"
#include "./dynamic_array.c"
#include "./arenas.c"


int main(void) {
  test_strings();
  test_ll();
  test_dynamic_array();
  test_arenas();
  return 0;
}
