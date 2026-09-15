@default:
  @just

test:
  ./test.sh

t: test

debug:
  #!/bin/bash
  set -e

  rm -rf ./build
  mkdir -p ./build
  ${fileDirname}

  gcc \
    --std=gnu99 \
    -Werror \
    -Wall \
    -Wconversion \
    -fsanitize=undefined \
    ./test/test.c \
    -I. \
    -g \
    -o \
    ./build/test_lib
  gdb ./build/test_lib


d: debug
