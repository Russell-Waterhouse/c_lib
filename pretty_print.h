#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

void print_green(char* msg);
void print_red(char* msg);

#endif
#ifdef PRETTY_PRINT_IMPLEMENTATION

void print_red(char* msg) {
  printf("\033[0;31m%s\033[0m\n", msg);
}

void print_green(char* msg) {
  printf("\x1B[32m%s\033[0m\n", msg);
}

#endif
