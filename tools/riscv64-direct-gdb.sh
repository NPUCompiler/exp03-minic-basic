#!/bin/bash

if [ $# -ne 2 ]; then
	echo "riscv64-direct-gdb.sh workspacefolder arm32filename"
	exit 1
fi

# 交叉编译程序成RISCV64程序
riscv64-linux-gnu-gcc -march=rv64gc -S --include "$1/tests/std.h" -o "$1/tests/$2.s" "$1/tests/$2.c"
riscv64-linux-gnu-gcc -march=rv64gc -g -static -o "$1/tests/$2" "$1/tests/$2.s" "$1/tests/std.c"

echo "Now run gdb in another window"
echo "Now gdb start"

# 通过qemu运行，同时开启gdb server功能
qemu-riscv64-static -g 1234 "$1/tests/$2"

echo
echo "Now gdb end"

exit 0