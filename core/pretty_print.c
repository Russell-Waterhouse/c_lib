#include <stdio.h>
#include "pretty_print.h"

void print_red(char* msg) {
  printf("\033[0;31m%s\033[0m\n", msg);
}

void print_green(char* msg) {
  printf("\x1B[32m%s\033[0m\n", msg);
}

