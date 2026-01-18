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
  ./memory/*.c \
  ./types/*.c \
  ./data_structures/*.c \
  ./core/*.c \
  ./io/*.c \
  -I. \
  -o \
  ./build/test_lib

./build/test_lib


