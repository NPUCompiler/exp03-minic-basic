///
/// @file CodeGeneratorAsm.cpp
/// @brief 后端汇编代码生成器接口的实现
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
#include "CodeGenerator.h"
#include "CodeGeneratorAsm.h"
#include "Module.h"
#include "Function.h"

/// @brief 构造函数
CodeGeneratorAsm::CodeGeneratorAsm(Module * _module) : CodeGenerator(_module)
{}

/// @brief .text代码段，主要存放CPU指令，以函数为单位
void CodeGeneratorAsm::genCodeSection()
{
    // 重新设置为0
    labelIndex = 0;

    // 遍历所有的函数，以函数为单位，产生指令
    for (auto func: module->getFunctionList()) {

        if (!func->isBuiltin()) {

            // 针对func产生汇编指令
            genCodeSection(func);
        }
    }
}

/// @brief 产生汇编文件
/// @param fp 要输出的文件
/// @return true:成功，false:失败
bool CodeGeneratorAsm::run()
{
    // 产生头
    genHeader();

    // 产生数据段，含初始化和未初始化数据
    genDataSection();

    // 产生代码段，即CPU指令，以函数为单位
    genCodeSection();

    return true;
}
