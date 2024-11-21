///
/// @file IRCode.cpp
/// @brief IR指令序列类头文件
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

#include <vector>

#include "Instruction.h"

/// @brief 中间IR指令序列管理类
class InterCode {

protected:
    /// @brief 指令块的指令序列
    std::vector<Instruction *> code;

public:
    /// @brief 构造函数
    InterCode() = default;

    /// @brief 析构函数
    ~InterCode();

    /// @brief 添加一个指令块，添加到尾部，并清除原来指令块的内容
    /// @param block 指令块，请注意加入后会自动清空block的指令
    void addInst(InterCode & block);

    /// @brief 添加一条中间指令
    /// @param inst IR指令
    void addInst(Instruction * inst);

    /// @brief 获取指令序列
    /// @return 指令序列
    std::vector<Instruction *> & getInsts();

    /// @brief 删除所有指令
    void Delete();
};
