///
/// @file InstSelectorArm32.h
/// @brief 指令选择器-ARM32
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

#include <map>
#include <vector>

#include "Function.h"
#include "ILocArm32.h"
#include "Instruction.h"
#include "PlatformArm32.h"
#include "SimpleRegisterAllocator.h"
#include "RegVariable.h"

using namespace std;

/// @brief 指令选择器-ARM32
class InstSelectorArm32 {

    /// @brief 所有的IR指令
    std::vector<Instruction *> & ir;

    /// @brief 指令变换
    ILocArm32 & iloc;

    /// @brief 要处理的函数
    Function * func;

protected:
    /// @brief 指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate(Instruction * inst);

    /// @brief NOP翻译成ARM32汇编
    /// @param inst IR指令
    void translate_nop(Instruction * inst);

    /// @brief 函数入口指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_entry(Instruction * inst);

    /// @brief 函数出口指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_exit(Instruction * inst);

    /// @brief 赋值指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_assign(Instruction * inst);

    /// @brief Label指令指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_label(Instruction * inst);

    /// @brief goto指令指令翻译成ARM32汇编
    /// @param inst IR指令
    void translate_goto(Instruction * inst);

    ///
    /// @brief 输出IR指令
    ///
    void outputIRInstruction(Instruction * inst);

    /// @brief IR翻译动作函数原型
    typedef void (InstSelectorArm32::*translate_handler)(Instruction *);

    /// @brief IR动作处理函数清单
    map<IRInstOperator, translate_handler> translator_handlers;

    ///
    /// @brief 简单的朴素寄存器分配方法
    ///
    SimpleRegisterAllocator & simpleRegisterAllocator;

    ///
    /// @brief 显示IR指令内容
    ///
    bool showLinearIR = false;

public:
    /// @brief 构造函数
    /// @param _irCode IR指令
    /// @param _func 函数
    /// @param _iloc 后端指令
    InstSelectorArm32(std::vector<Instruction *> & _irCode,
                      ILocArm32 & _iloc,
                      Function * _func,
                      SimpleRegisterAllocator & allocator);

    ///
    /// @brief 析构函数
    ///
    ~InstSelectorArm32();

    ///
    /// @brief 设置是否输出线性IR的内容
    /// @param show true显示，false显示
    ///
    void setShowLinearIR(bool show)
    {
        showLinearIR = show;
    }

    /// @brief 指令选择
    void run();
};
