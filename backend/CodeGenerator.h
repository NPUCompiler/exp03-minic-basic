///
/// @file CodeGenerator.h
/// @brief 代码生成器共同类的头文件
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
#pragma once

#include <cstdio>
#include <string>

#include "Module.h"

/// @brief 代码生成的一般类
class CodeGenerator {

public:
    /// @brief 构造函数
    /// @param _symtab 符号表
    CodeGenerator(Module * _module);

    /// @brief 析构函数
    virtual ~CodeGenerator() = default;

    /// @brief 代码产生器运行，结果保存到指定的文件中
    /// @param outFileName 输出内容所在文件
    /// @return true：成功，false：失败
    bool run(std::string outFileName);

    ///
    /// @brief 设置是否显示IR指令内容
    /// @param show true：显示，false：不显示
    ///
    void setShowLinearIR(bool show)
    {
        this->showLinearIR = show;
    }

protected:
    /// @brief 代码产生器运行，结果保存到指定的文件中
    /// @param fp 输出内容所在文件的指针
    /// @return true：成功，false：失败
    virtual bool run() = 0;

    ///
    /// @brief 一个C语言的文件对应一个Module
    ///
    Module * module;

    /// @brief 输出文件指针
    FILE * fp = nullptr;

    ///
    /// @brief 显示IR指令内容
    ///
    bool showLinearIR = false;
};
