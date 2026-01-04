#!/bin/bash
set -e

rm -rf ./build
mkdir -p ./build
${fileDirname}

gcc \
  --std=c89 \
  -Werror \
  -Wall \
  -Wconversion \
  -fsanitize=undefined \
  ./test/test.c \
  -I. \
  -o \
  ./build/test_lib

./build/test_lib


