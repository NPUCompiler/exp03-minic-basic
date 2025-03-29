///
/// @file ScopeStack.h
/// @brief 作用域栈管理
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-19
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-19 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#pragma once

#include <unordered_map>
#include <vector>

#include "Value.h"

///
/// @brief 变量作用域管理类，内部通过栈来实现
///
class ScopeStack {
    // 作用域栈

public:
    ///
    /// @brief 向当前的作用域中加入变量
    /// @param value 变量
    ///
    void insertValue(Value * value);

    ///
    /// @brief 从当前的作用域中查找指定的变量名
    /// @param  name 变量名
    /// @return Value* 变量对象，若没有，则返回空指针
    ///
    Value * findCurrentScope(std::string name);

    ///
    /// @brief 获取当前的作用域栈的层号
    /// @return int 层号
    ///
    int getCurrentScopeLevel();

    ///
    /// @brief 逐层级遍历作用域检查变量是否存在
    /// @param  name 变量名
    /// @return Value* 变量对象。若没有，则返回空指针
    ///
    Value * findAllScope(std::string name);

    ///
    /// @brief 进入作用域
    ///
    void enterScope();

    ///
    /// @brief 离开作用域
    ///
    void leaveScope();

protected:
    ///
    /// @brief 变量作用域栈，最外层用vector来模拟栈，每一层用unordered_map来实现，变量名为key，变量为value
    ///
    std::vector<std::unordered_map<std::string, Value *>> valueStack;
};
