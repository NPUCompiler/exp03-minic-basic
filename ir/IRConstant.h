///
/// @file IRConstant.h
/// @brief DragonIR定义的符号常量
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

#define IR_GLOBAL_VARNAME_PREFIX "@"
#define IR_LOCAL_VARNAME_PREFIX "%l"
#define IR_TEMP_VARNAME_PREFIX "%t"
#define IR_MEM_VARNAME_PREFIX "%m"
#define IR_LABEL_PREFIX ".L"

#define IR_KEYWORD_DECLARE "declare"
#define IR_KEYWORD_DEFINE "define"
#define IR_KEYWORD_ADD_I "add"
#define IR_KEYWORD_SUB_I "sub"
