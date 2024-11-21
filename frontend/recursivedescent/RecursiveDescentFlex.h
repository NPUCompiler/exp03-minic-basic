///
/// @file RecursiveDescentFlex.h
/// @brief 词法分析的头文件，不借助工具实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#pragma once

#include <stdio.h>

// 行号信息
extern int rd_line_no;

// 输入文件指针
extern FILE * rd_filein;

/// 识别词法
int rd_flex();
