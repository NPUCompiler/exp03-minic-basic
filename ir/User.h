///
/// @file User.h
/// @brief 使用Value的User，该User也是Value。函数、指令都是User
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

#include <unordered_map>
#include <list>

#include "Value.h"
#include "Use.h"

///
/// @brief 本身代表一个Value，这个Value可通过其中的操作数计算得到
///
/// User是LLVM IR中表示使用Value的类。
/// User可以是指令(Instruction)、常量表达式(ConstantExpr)、全局变量(GlobalVariable)等。
/// User持有对Value的引用，并且可以有多个Value作为其操作数(Operands)
///
class User : public Value {

public:
    ///
    /// @brief 构造函数
    /// @param _type  类型
    /// @param _name  名称
    ///
    User(Type * _type);

    ///
    /// @brief Get the Operands object
    /// @return std::vector<Use *>&
    ///
    std::vector<Use *> & getOperands();

    ///
    /// @brief 取得操作数
    /// @return std::vector<Value *>
    ///
    std::vector<Value *> getOperandsValue();

    ///
    /// @brief 获取操作数的个数
    /// @return int32_t 个数
    ///
    int32_t getOperandsNum();

    ///
    /// @brief 获取指定的操作数
    /// @param pos 位置
    /// @return Value* 操作数
    ///
    Value * getOperand(int32_t pos);

    ///
    /// @brief 更新指定Pos的Value
    /// @param pos 位置
    /// @param val 操作数
    ///
    void setOperand(int32_t pos, Value * val);

    ///
    /// @brief 增加操作数，或者说本身的值由这些操作数来计算得到
    /// @param pos 索引位置
    /// @param val 值
    ///
    void addOperand(Value * val);

    ///
    /// @brief 清除指定的操作数
    /// @param pos 操作数的索引
    ///
    void removeOperand(int pos);

    ///
    /// @brief 清除指定的Use
    /// @param val 操作指定的操作数
    ///
    void removeOperand(Value * val);

    ///
    /// @brief 直接清除操作数的元素
    /// @param use 指定的元素use
    ///
    void removeOperandRaw(Use * use);

    ///
    /// @brief 清除指定的Use
    /// @param use define-use边
    ///
    void removeUse(Use * use);

    ///
    /// @brief 清除所有的操作数
    ///
    void clearOperands();
};