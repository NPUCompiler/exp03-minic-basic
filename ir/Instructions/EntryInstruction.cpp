///
/// @file EntryInstruction.cpp
/// @brief 函数的入口指令
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
#include "EntryInstruction.h"
#include "Types/VoidType.h"

/// @brief return语句指令
EntryInstruction::EntryInstruction(Function * _func)
    : Instruction(_func, IRInstOperator::IRINST_OP_ENTRY, VoidType::getType())
{}

/// @brief 转换成字符串
void EntryInstruction::toString(std::string & str)
{
    str = "entry";
}
