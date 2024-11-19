///
/// @file Use.h
/// @brief Use类定义了一条Define-Use边，usee为定义的Value，user代表使用该Value的User
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

#include <cstdint>
#include <vector>

class User;
class Value;

///
/// @brief  A Use represents the edge between a Value definition and its users.
///
class Use {

protected:
    ///
    /// @brief 指向要使用的value
    ///
    Value * usee = nullptr;

    ///
    /// @brief 使用val的使用者或者用户
    ///
    User * user = nullptr;

public:
    /**
     * 构建函数，构建一条define-use的边
     * <br>
     * The constructor DOES NOT automatically insert the Use link constructed to neither usee's [uses]
     * field nor the user's [operands] field, which are left for the caller to manually handle.
     * @param value The value being used.
     * @param user  The user value.
     */
    Use(Value * _value, User * _user) : usee(_value), user(_user)
    {}

    ///
    /// @brief 获取值
    /// @return Value *
    ///
    operator Value *() const
    {
        return usee;
    }

    ///
    /// @brief 获取Define-Use边的使用者
    /// @return User* 使用者
    ///
    [[nodiscard]] User * getUser() const
    {
        return user;
    }

    ///
    /// @brief 获取Define-Use的Define，即被使用者
    /// @return Value*
    ///
    [[nodiscard]] Value * getUsee() const
    {
        return usee;
    }

    ///
    /// @brief 不再使用Use原来的Value，更新为新的Value
    /// @param newVal 新的Value
    ///
    void setUsee(Value * newVal);

    ///
    /// @brief def-use边取消，但不会删除该Use
    ///
    void remove();
};