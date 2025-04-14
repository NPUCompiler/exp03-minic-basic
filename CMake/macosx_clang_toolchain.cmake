# 指定目标系统
SET(CMAKE_SYSTEM_NAME Darwin)

# 指定编译器
SET(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
SET(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

# 指定搜索的根路径
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # 使用本机程序
