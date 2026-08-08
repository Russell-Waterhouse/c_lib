#ifndef RW_CLIB_DEBUGGING
#define RW_CLIB_DEBUGGING

void breakpoint() {
  __asm__ volatile("int $3"); // Breakpoint in gdb
}

void debugger() {
  __asm__ volatile("int $3"); // Breakpoint in gdb
}

#endif // !RW_CLIB_DEBUGGING
