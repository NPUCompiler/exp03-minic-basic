///
/// @file Constant.h
/// @brief 常量类
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

#include "User.h"

///
/// @brief 常量是在运行时不可更改(immutable)的值
/// 常量可以是基本类型的值，如整数或浮点类型的值，也可以是复杂的，如结构体或数组
/// 或者是基于表达式运算得到的，可以用其它常量的值计算得到
/// ! 函数是常量，这是因为函数的地址运行时不可更改
/// ! 全局变量也是常量，也是因为全局变量的地址运行时不可更改
///
class Constant : public User {

protected:
    ///
    /// @brief 构造函数
    /// @param _type  类型
    ///
    Constant(Type * _type) : User(_type)
    {}
};
