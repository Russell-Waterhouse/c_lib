#!/bin/bash
set -e

rm -rf ./build
mkdir -p ./build
${fileDirname}

gcc \
  --std=c99 \
  -Werror \
  -Wall \
  -Wconversion \
  -fsanitize=undefined \
  ./test/test.c \
  -I. \
  -o \
  ./build/test_lib

valgrind --leak-check=full --show-leak-kinds=all ./build/test_lib

