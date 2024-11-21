///
/// @file CodeGeneratorAsm.h
/// @brief 后端汇编代码生成器接口的头文件
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
#include <cstring>

#include "CodeGenerator.h"

/// @brief 生成汇编的代码生成器共同类
class CodeGeneratorAsm : public CodeGenerator {

protected:
    /// @brief .text代码段，主要存放CPU指令，以函数为单位
    void genCodeSection(FILE * fp);

public:
    /// @brief 构造函数
    CodeGeneratorAsm(Module * module);

    /// @brief 析构函数
    ~CodeGeneratorAsm() override = default;

    /// @brief 产生汇编头部分
    virtual void genHeader() = 0;

    /// @brief 全局变量Section，主要包含初始化的和未初始化过的
    virtual void genDataSection() = 0;

    /// @brief 针对函数进行汇编指令生成，放到.text代码段中
    /// @param func 要处理的函数
    virtual void genCodeSection(Function * func) = 0;

    /// @brief 寄存器分配
    /// @param func 要处理的函数
    virtual void registerAllocation(Function * func) = 0;

protected:
    /// @brief 产生汇编文件
    /// @return true:成功，false:失败
    bool run() override;

    /// @brief 汇编指令生成，放到.text代码段中
    void genCodeSection();

    ///
    /// @brief Label索引编号，要求文件级别的编号，而不是函数级别的编号
    ///
    int64_t labelIndex = 0;
};
