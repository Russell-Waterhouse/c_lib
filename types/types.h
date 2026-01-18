#ifndef TYPES_H

#define TYPES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef enum {
  FAIL = 0,
  SUCCESS = 1
} Result;

typedef enum {
  ERR_NONE = 0,
  ERR_INVALID_ARG = 1,
  ERR_MEM_ALLOC_FAIL = 2,
  ERR_IO_DISK_FAIL = 3,
  ERR_IO_NETWORK_FAIL = 4,
} ErrCode;

typedef struct {
  ErrCode code;
  const char* msg;
} Error;

typedef struct {
  Result status;
  Error err;
} IOResult; /* TODO: Not sure I'm happy with this name. */

typedef struct {
  Result status;
  Error err;
  u64 res;
} u64Result;

union PointerOrErrror {
  Error err;
  void* res;
};

typedef struct {
  Result status;
  union PointerOrErrror val;
} PointerResult;

#define DYNAMIC_ARRAY_START_SIZE 256
#endif
