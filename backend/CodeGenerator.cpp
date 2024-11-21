///
/// @file CodeGenerator.cpp
/// @brief 代码生成器共同类的实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#include <cstdio>
#include <string>

#include "Module.h"
#include "CodeGenerator.h"

/// @brief 构造函数
/// @param _module 符号表:模块
CodeGenerator::CodeGenerator(Module * _module) : module(_module)
{}

/// @brief 代码产生器运行，结果保存到指定的文件中
/// @param outFileName 输出内容所在文件
/// @return true：成功，false：失败
bool CodeGenerator::run(std::string outFileName)
{
    // 打开文件，也可以以C++的方式打开文件进行操作
    // 这里主要便于C语言学习的学生
    if (!outFileName.empty()) {
        // 指定文件非空时，则创建文件
        fp = fopen(outFileName.c_str(), "w");
        if (nullptr == fp) {
            printf("open file(%s) failed", outFileName.c_str());
            return false;
        }
    } else {
        fp = nullptr;
    }

    // 执行真正的代码
    const bool result = run();

    // 关闭文件
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }

    return result;
}
