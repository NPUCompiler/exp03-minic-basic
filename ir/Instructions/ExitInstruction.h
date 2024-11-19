///
/// @file ExitInstruction.h
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
#pragma once

#include "Instruction.h"

///
/// @brief 函数的出口指令，用来指代函数返回指令
///
class ExitInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    /// @param _func 所属的函数
    /// @param result 函数的返回值
    ExitInstruction(Function * _func, Value * result = nullptr);

    /// @brief 转换成字符串
    void toString(std::string & str) override;
};
