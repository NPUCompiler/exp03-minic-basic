///
/// @file Use.cpp
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

#include "Use.h"
#include "User.h"

///
/// @brief 不再使用Use原来的Value，更新为新的Value，导致删除原来的边，新加一条边
/// @param newVal 新的Value
///
void Use::setUsee(Value * newVal)
{
    this->usee->removeUse(this);
    this->usee = newVal;
    this->usee->addUse(this);
}

///
/// @brief 移除def-use边，需要两头分别清理
/// ! 请注意该函数并没有删除Use，需要自行清理资源
///
void Use::remove()
{
    usee->removeUse(this);
    user->removeOperandRaw(this);
}