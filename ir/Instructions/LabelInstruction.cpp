///
/// @file LabelInstruction.cpp
/// @brief Label指令
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#include "VoidType.h"

#include "LabelInstruction.h"

///
/// @brief 构造函数
/// @param _func 所属函数
///
LabelInstruction::LabelInstruction(Function * _func)
    : Instruction(_func, IRInstOperator::IRINST_OP_LABEL, VoidType::getType())
{}

/// @brief 转换成字符串
/// @param str 返回指令字符串
void LabelInstruction::toString(std::string & str)
{
    str = IRName + ":";
}
