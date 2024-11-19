///
/// @file IntegerType.cpp
/// @brief 整型类型类，可描述1位的bool类型或32位的int类型
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

#include "IntegerType.h"

///
/// @brief 唯一的VOID类型实例
///
IntegerType * IntegerType::oneInstanceBool = new IntegerType(1);
IntegerType * IntegerType::oneInstanceInt = new IntegerType(32);

///
/// @brief 获取类型bool
/// @return VoidType*
///
IntegerType * IntegerType::getTypeBool()
{
    return oneInstanceBool;
}

///
/// @brief 获取类型int
/// @return VoidType*
///
IntegerType * IntegerType::getTypeInt()
{
    return oneInstanceInt;
}
