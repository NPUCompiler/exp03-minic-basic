#!/bin/bash

if [ $# -ne 2 ]; then
	echo "arm64-build-gdb.sh workspacefolder arm32filename"
	exit 1
fi

# 生成ARM64汇编语言
"$1/build/minic" -S -A -o "$1/tests/$2.s" "$1/tests/$2.c"

# 交叉编译程序成ARM64程序
aarch64-linux-gnu-gcc -march=armv8-a -g -static --include "$1/tests/std.h" -o "$1/tests/$2" "tests/$2.s" "$1/tests/std.c"

echo "Now run gdb in another window"
echo "Now gdb start"

# 通过qemu运行，同时开启gdb server功能
qemu-aarch64-static -g 1234 "$1/tests/$2"

echo
echo "Now gdb end"

exit 0