#ifndef ARENAS_H
#define ARENAS_H

#include <stdlib.h>
#include "./types.h"
typedef struct Arena {
  void* start_position;
  void* current_position;
  size_t len;
  struct Arena* next;
} Arena;

typedef struct {
  Result status;
  Error err;
  Arena* arena;
} ArenaResult;

#define ARENA_ALIGN (sizeof(void*))

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

ArenaResult arena_create(size_t size);
PointerResult arena_push(Arena* arena, size_t size);
Result arena_free(Arena* arena);

#endif // ARENAS_H
#ifdef ARENAS_IMPLEMENTATION

#include "types.h"
#include <stdlib.h>

ArenaResult arena_create(size_t len) {
  ArenaResult res;
  if (len < 1) {
    res.status = FAIL;
    res.err.code = ERR_INVALID_ARG;
    res.err.msg = "Cannot create an arena with a len of 0";
    return res;
  }
  res.arena = (Arena*)malloc(sizeof(Arena));
  if (NULL == res.arena) {
    res.status = FAIL;
    res.err.code = ERR_MEM_ALLOC_FAIL;
    res.err.msg = "Failed to malloc arena struct in arena_create";
    return res;
  }
  res.arena->start_position = malloc(len);
  if (NULL == res.arena->start_position) {
    res.status = FAIL;
    res.err.code = ERR_MEM_ALLOC_FAIL;
    res.err.msg = "Failed to alloc arena buffer";
    free(res.arena);
    res.arena = NULL;
    return res;
  }
  res.status = SUCCESS;
  res.arena->current_position = res.arena->start_position;
  res.arena->len = len;
  res.arena->next = NULL;

  return res;
}

PointerResult arena_push(Arena* arena, size_t size) {
  PointerResult p;
  if (arena->len == 0 || arena->start_position == NULL) {
    p.status = FAIL;
    p.val.err.code = ERR_INVALID_ARG;
    p.val.err.msg = "Passed a null arena to arena_push";
    return p;
  }

  if (NULL != arena->next) {
    return arena_push(arena->next, size);
  }

  void* aligned_current_pos = (void*)(((u64)(arena->current_position) + ((u64)(ARENA_ALIGN) - 1)) & (~((u64)(ARENA_ALIGN) - 1)));

  void* new_pos = aligned_current_pos + size;

  if (new_pos >= arena->current_position + arena->len) {
    ArenaResult r = arena_create((2 * arena->len ) + (2 * size));
    if (SUCCESS != r.status) {
      p.status = FAIL;
      p.val.err.code = r.err.code;
      p.val.err.msg = r.err.msg;
      return p;
    }
    arena->next = r.arena;
    return arena_push(arena->next, size);
  }
  arena->current_position = new_pos;

  p.status = SUCCESS;
  p.val.res = (u8*)aligned_current_pos;

  return p;
}

Result arena_free(Arena* arena) {
  Result next_free_res = SUCCESS;
  if (NULL == arena->start_position) {
    return FAIL;
  }
  free(arena->start_position);
  arena->start_position = NULL;
  if (NULL != arena->next) {
    next_free_res = arena_free(arena->next);
    arena->next = NULL;
  }

  free(arena);
  arena = NULL;
  return SUCCESS && next_free_res;
}

#endif
