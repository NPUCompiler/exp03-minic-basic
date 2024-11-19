///
/// @file RegVariable.h
/// @brief 寄存器变量类，用于后端
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

#include "Value.h"

/// @brief 整型寄存器Value
class RegVariable : public Value {

public:
    /// @brief 整型寄存器型Value
    /// \param val
    explicit RegVariable(Type * _type, std::string _name, int32_t _reg_no) : Value(_type)
    {
        this->name = _name;
        regId = _reg_no;
    }

    ///
    /// @brief 获得分配的寄存器编号或ID
    /// @return int32_t 寄存器编号
    ///
    int32_t getRegId() override
    {
        return regId;
    }

    /// @brief 获取名字
    /// @return 变量名
    [[nodiscard]] std::string getIRName() const override
    {
        return name;
    }

private:
    /// @brief 寄存器编号，-1表示没有分配寄存器，大于等于0代表是寄存器型Value
    int32_t regId = -1;
};
