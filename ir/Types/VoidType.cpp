///
/// @file VoidType.cpp
/// @brief void类型描述类
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

#include "VoidType.h"

///
/// @brief 唯一的VOID类型实例
///
VoidType * VoidType::oneInstance = new VoidType();

///
/// @brief 获取类型
/// @return VoidType*
///
VoidType * VoidType::getType()
{
    return oneInstance;
}