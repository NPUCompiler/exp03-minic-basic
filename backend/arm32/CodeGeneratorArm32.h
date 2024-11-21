///
/// @file CodeGeneratorArm32.h
/// @brief ARM32的后端处理头文件
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
#include "CodeGeneratorAsm.h"
#include "SimpleRegisterAllocator.h"

class CodeGeneratorArm32 : public CodeGeneratorAsm {

public:
    /// @brief 构造函数
    /// @param tab 符号表
    CodeGeneratorArm32(Module * module);

    /// @brief 析构函数
    ~CodeGeneratorArm32() override;

protected:
    /// @brief 产生汇编头部分
    void genHeader() override;

    /// @brief 全局变量Section，主要包含初始化的和未初始化过的
    void genDataSection() override;

    /// @brief 针对函数进行汇编指令生成，放到.text代码段中
    /// @param func 要处理的函数
    void genCodeSection(Function * func) override;

    /// @brief 寄存器分配
    /// @param func 要处理的函数
    void registerAllocation(Function * func) override;

    /// @brief 栈空间分配
    /// @param func 要处理的函数
    void stackAlloc(Function * func);

    /// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
    /// @param func 要处理的函数
    void adjustFuncCallInsts(Function * func);

    /// @brief 寄存器分配前对形参指令调整，便于栈内空间分配以及寄存器分配
    /// @param func 要处理的函数
    void adjustFormalParamInsts(Function * func);

    ///
    /// @brief 获取IR变量相关信息字符串
    /// @param str
    ///
    void getIRValueStr(Value * val, std::string & str);

private:
    ///
    /// @brief 简单的朴素寄存器分配方法
    ///
    SimpleRegisterAllocator simpleRegisterAllocator;
};
