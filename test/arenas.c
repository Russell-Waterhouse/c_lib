#include "../types.h"
#include "../arenas.h"


Result test_create() {
  Arena a = {0};
  Result r = arena_create(&a, KiB(4));
  arena_free(&a);
  return r;
}

Result test_push() {
  Arena a = {0};
  Result r1 = arena_create(&a, KiB(4));
  PointerResult r2 = arena_push(&a, sizeof(u8));
  PointerResult r3 = arena_push(&a, sizeof(u16));
  PointerResult r4 = arena_push(&a, sizeof(u32));
  PointerResult r5 = arena_push(&a, sizeof(u64));
  if (
      r1 != SUCCESS ||
      r2.status != SUCCESS ||
      r3.status != SUCCESS ||
      r4.status != SUCCESS ||
      r5.status != SUCCESS
  ) {
    arena_free(&a);
    return FAIL;
  }

  u8 *ru8 = r2.val.res;
  u16 *ru16 = r3.val.res;
  u32 *ru32 = r4.val.res;
  u64 *ru64 = r5.val.res;

  *ru8 = 0xFF;
  *ru16 = 0;
  *ru32 = 0xFFFFFF;
  *ru64 = 0;
  if (
      *ru8 != 0xFF ||
      *ru16 != 0 ||
      *ru32 != 0xFFFFFF ||
      *ru64 != 0
  ) {
    arena_free(&a);
    return FAIL;
  }

  PointerResult rarr = arena_push(&a, sizeof(u16) * 255);
  if (rarr.status != SUCCESS) {
    arena_free(&a);
    return FAIL;
  }

  u16 * arr = (u16*)rarr.val.res;
  u8 i;
  for(i = 0; i < 255; i++) {
    arr[i] = i;
  }

  for(i = 0; i < 255; i++) {
    if (arr[i] != i) {
      arena_free(&a);
      return FAIL;
    }
  }

  arena_free(&a);
  return SUCCESS;
}

void test_arenas(void) {
  puts("Starting arena allocator tests.");
  if (
      test_create() == SUCCESS &&
      test_push() == SUCCESS
  ) {
    puts("\x1B[32mTests completed successfully!\033[0m\n");
    exit(0);
  }


  puts("\033[0;31mThere were test failures.\033[0m\n");
  exit(1);
}
