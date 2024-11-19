/**
 *@file CodeGenerator.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 代码生成器共同类的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
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

protected:
    /// @brief 代码产生器运行，结果保存到指定的文件中
    /// @param fp 输出内容所在文件的指针
    /// @return true：成功，false：失败
    virtual bool run() = 0;

    /// @brief 符号表
    Module * module;

    /// @brief 输出文件指针
    FILE * fp = nullptr;
};
