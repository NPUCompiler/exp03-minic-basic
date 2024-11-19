#!/bin/bash
# 用于便捷测试抽象语法树的脚本,读取tests/test.c文件，生成抽象语法树.png，保存到result文件夹下(如果不存在，则自动创建)。
# 使用指令 "chmod +x ./tools/Dast.sh" 给脚本权限
# 使用方式 ./tools/Dast.sh test 
# 且test应在/minic/tests目录下,test为测试输入文件、输出图片的名字。不需要包含.c的后缀 
# 使用实例： "./tools/Dast.sh test1-1"
# "Dast.sh" 中的D表示使用 递归下降分析法

# 检查是否提供了name参数
if [ "$#" -ne 1 ]; then
    echo "Usage: ./Tast <name>"
    exit 1
fi

# 获取name参数
name=$1

# 检查是否已经存在名为 "result" 的文件夹
if [ ! -d "result" ]; then
  # 如果不存在，就创建它
  mkdir result
  echo "Folder 'result' has been created."
fi

# 构建你要执行的命令
command="./cmake-build-debug/minic -S -D  -T -o ./result/${name}.png ./tests/${name}.c"

# 执行命令
$command