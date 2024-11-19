///
/// @file LabelType.cpp
/// @brief Label名称符号类
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

#include "LabelType.h"

///
/// @brief 唯一的VOID类型实例
///
LabelType * LabelType::oneInstance = new LabelType();

///
/// @brief 获取类型
/// @return VoidType*
///
LabelType * LabelType::getType()
{
    return oneInstance;
}