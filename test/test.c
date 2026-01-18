#include "./strings.c"
#include "./linked_list.c"
#include "./dynamic_array.c"
#include "./arenas.c"
#include "./pdf.c"
#include "./dictionary.c"


int main(void) {
  test_strings();
  test_ll();
  test_dynamic_array();
  test_arenas();
  test_pdf();
  test_dictionary();
  return 0;
}

