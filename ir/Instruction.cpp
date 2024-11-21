///
/// @file Instruction.cpp
/// @brief IR指令类实现
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
#include <string>

#include "Instruction.h"
#include "Function.h"

/// @brief 构造函数
/// @param op
/// @param result
/// @param srcVal1
/// @param srcVal2
Instruction::Instruction(Function * _func, IRInstOperator _op, Type * _type) : User(_type), op(_op), func(_func)
{}

/// @brief 获取指令操作码
/// @return 指令操作码
IRInstOperator Instruction::getOp()
{
    return op;
}

/// @brief 转换成字符串
/// @param str 转换后的字符串
void Instruction::toString(std::string & str)
{
    // 未知指令
    str = "Unkown IR Instruction";
}

/// @brief 是否是Dead指令
bool Instruction::isDead()
{
    return dead;
}

/// @brief 设置指令是否是Dead指令
/// @param _dead 是否是Dead指令，true：Dead, false: 非Dead
void Instruction::setDead(bool _dead)
{
    dead = _dead;
}

///
/// @brief 获取当前指令所在函数
/// @return Function* 函数对象
///
Function * Instruction::getFunction()
{
    return func;
}

///
/// @brief 检查指令是否有值
/// @return true
/// @return false
///
bool Instruction::hasResultValue()
{
    return !type->isVoidType();
}