#include "./arenas.h"
#include "types.h"
#include <stdlib.h>

Result arena_create(Arena* arena, size_t len) {
  arena->start_position = malloc(len);
  if (NULL == arena->start_position) {
    return FAIL;
  }
  arena->current_position = arena->start_position;
  arena->len = len;
  arena->next = NULL;

  return SUCCESS;
}

PointerResult arena_push(Arena* arena, size_t size) {
  PointerResult p;
  if (arena->len == 0 || arena->start_position == NULL) {
    p.status = FAIL;
    p.val.err.err_code = INVALID_ARG;
    p.val.err.err_msg = "Passed a null arena to arena_push";
    return p;
  }

  if (NULL != arena->next) {
    return arena_push(arena->next, size);
  }

  void* aligned_current_pos = (void*)(((u64)(arena->current_position) + ((u64)(ARENA_ALIGN) - 1)) & (~((u64)(ARENA_ALIGN) - 1)));

  void* new_pos = aligned_current_pos + size;

  if (new_pos >= arena->current_position + arena->len) {
    arena->next = malloc(sizeof(*arena));
    if (NULL == arena->next) {
      p.status = FAIL;
      p.val.err.err_code = MEM_ALLOC_FAIL;
      p.val.err.err_msg = "Failed to create next arena in linked list";
      return p;
    }
    Result create_result = arena_create(arena->next, size * 2);
    if (FAIL == create_result) {
      p.status = FAIL;
      p.val.err.err_code = MEM_ALLOC_FAIL;
      p.val.err.err_msg = "Failed to initialize next arena in linked list";
      return p;
    }
    return arena_push(arena->current_position, size);
  }
  arena->current_position = new_pos;

  p.status = SUCCESS;
  p.val.res = (u8*)aligned_current_pos;

  return p;
}

Result arena_free(Arena* arena) {
  if (NULL == arena->start_position) {
    return FAIL;
  }
  /* TODO: free the actual arena structs themselves */
  free(arena->start_position);
  if (NULL != arena->next) {
    return arena_free(arena->next);
  }
  return SUCCESS;
}

