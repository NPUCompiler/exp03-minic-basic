#!/bin/bash

if [ $# -ne 2 ]; then
	echo "arm32-direct-gdb.sh workspacefolder arm32filename"
	exit 1
fi

# 交叉编译程序成ARM32程序
arm-linux-gnueabihf-gcc -S --include "$1/tests/std.h" -o "$1/tests/$2.s" "$1/tests/$2.c"
arm-linux-gnueabihf-gcc -g -static -o "$1/tests/$2" "$1/tests/$2.s" "$1/tests/std.c"

echo "Now run gdb in another window"
echo "Now gdb start"

# 通过qemu运行，同时开启gdb server功能
qemu-arm-static -g 1234 "$1/tests/$2"

echo
echo "Now gdb end"

exit 0