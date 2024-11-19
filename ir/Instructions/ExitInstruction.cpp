///
/// @file ExitInstruction.cpp
/// @brief 函数出口或返回指令
///
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

#include "ExitInstruction.h"

/// @brief return语句指令
/// @param _result 返回结果值
ExitInstruction::ExitInstruction(Function * _func, Value * _result)
    : Instruction(_func, IRInstOperator::IRINST_OP_EXIT, VoidType::getType())
{
    if (_result != nullptr) {
        addOperand(_result);
    }
}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void ExitInstruction::toString(std::string & str)
{
    if (getOperandsNum() == 0) {
        str = "exit void";
    } else {
        Value * src1 = getOperand(0);
        str = "exit " + src1->getIRName();
    }
}
