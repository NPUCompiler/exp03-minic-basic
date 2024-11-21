///
/// @file PlatformArm32.h
/// @brief  ARM32平台相关头文件
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

#include <string>

#include "RegVariable.h"

// 在操作过程中临时借助的寄存器为ARM32_TMP_REG_NO
#define ARM32_TMP_REG_NO 10

// 栈寄存器SP和FP
#define ARM32_SP_REG_NO 13
#define ARM32_FP_REG_NO 11

// 函数跳转寄存器LX
#define ARM32_LX_REG_NO 14

/// @brief ARM32平台信息
class PlatformArm32 {

    /// @brief 循环左移两位
    /// @param num
    static void roundLeftShiftTwoBit(unsigned int & num);

    /// @brief 判断num是否是常数表达式，8位数字循环右移偶数位得到
    /// @param num
    /// @return
    static bool __constExpr(int num);

public:
    /// @brief 同时处理正数和负数
    /// @param num
    /// @return
    static bool constExpr(int num);

    /// @brief 判定是否是合法的偏移
    /// @param num
    /// @return
    static bool isDisp(int num);

    /// @brief 判断是否是合法的寄存器名
    /// @param name 寄存器名字
    /// @return 是否是
    static bool isReg(std::string name);

    /// @brief 最大寄存器数目
    static const int maxRegNum = 16;

    /// @brief 可使用的通用寄存器的个数r0-r10
    static const int maxUsableRegNum = 11;

    /// @brief 寄存器的名字，r0-r15
    static const std::string regName[maxRegNum];

    /// @brief 对寄存器R0分配Value，记录位置
    static RegVariable * intRegVal[PlatformArm32::maxRegNum];
};
