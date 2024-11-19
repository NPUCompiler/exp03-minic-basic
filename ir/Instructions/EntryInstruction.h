///
/// @file EntryInstruction.h
/// @brief 函数的入口指令
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
/// @brief 函数入口指令
///
class EntryInstruction : public Instruction {

public:
    ///
    /// @brief 构造函数
    ///
    EntryInstruction(Function * _func);

    ///
    /// @brief 转换成IR指令字符串
    ///
    void toString(std::string & str) override;
};
