/**
 * @file IRCode.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 中间IR指令序列管理类的头文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
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
