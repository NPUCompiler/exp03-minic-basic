///
/// @file ScopeStack.cpp
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
#include "ScopeStack.h"

///
/// @brief 进入作用域
///
void ScopeStack::enterScope()
{
    // 在栈顶新加入一层，没有变量
    std::unordered_map<std::string, Value *> valueMap;
    valueStack.emplace_back(valueMap);
}

///
/// @brief 离开作用域
///
void ScopeStack::leaveScope()
{
    valueStack.pop_back();
}

///
/// @brief 向当前的作用域中加入变量
/// @param value 变量
///
void ScopeStack::insertValue(Value * value)
{
    valueStack.back().insert(make_pair(value->getName(), value));
}

///
/// @brief 从当前的作用域中查找指定的变量名
/// @param  name 变量名
/// @return Value* 变量对象，若没有，则返回空指针
///
Value * ScopeStack::findCurrentScope(std::string name)
{
    // 在栈顶的作用域中查找，即当前作用域
    auto it = valueStack.back().find(name);
    if (it != valueStack.back().end()) {
        return it->second;
    }
    return nullptr;
}

///
/// @brief 逐层级遍历作用域检查变量是否存在
/// @param  name 变量名
/// @return Value* 变量对象。若没有，则返回空指针
///
Value * ScopeStack::findAllScope(std::string name)
{
    // 模拟栈操作，从栈顶开始查找
    for (auto it = valueStack.rbegin(); it != valueStack.rend(); ++it) {
        auto p = it->find(name);
        if (p != it->end()) {
            return p->second;
        }
    }
    return nullptr;
}

///
/// @brief 获取当前的作用域栈的层号
/// @return int 层号，从0开始
///
int ScopeStack::getCurrentScopeLevel()
{
    return valueStack.size() - 1;
}
