#ifndef RW_CLIB_DEBUGGING
#define RW_CLIB_DEBUGGING

// Breakpoint in gdb
#define debugger __asm__ volatile("int $3");
// Breakpoint in gdb
#define breakpoint __asm__ volatile("int $3");

#endif // !RW_CLIB_DEBUGGING
