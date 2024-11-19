/**
 * @file RecursiveDescentFlex.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 词法分析的手动实现头文件
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdio.h>

// 行号信息
extern int rd_line_no;

// 输入文件指针
extern FILE * rd_filein;

/// 识别词法
int rd_flex();
