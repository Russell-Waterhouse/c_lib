#include "casts.h"

u64Result i64_to_u64(i64 i) {
  u64Result res;
  if (i < 0) {
    res.status = FAIL;
    res.err.code = ERR_INVALID_ARG;
    res.err.msg = "Attempting to cast negative i64 to u64";
    return res;
  }
  res.res = (u64)i;
  res.status = SUCCESS;
  return res;
}

