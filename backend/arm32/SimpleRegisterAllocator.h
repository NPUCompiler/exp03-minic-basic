///
/// @file SimpleRegisterAllocator.h
/// @brief 简单或朴素的寄存器分配器
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

#include <vector>

#include "BitMap.h"
#include "Value.h"
#include "PlatformArm32.h"

class SimpleRegisterAllocator {

public:
    ///
    /// @brief Construct a new Simple Register Allocator object
    ///
    SimpleRegisterAllocator();

    ///
    /// @brief 尝试按指定的寄存器编号进行分配，若能分配，则直接分配，否则从小达到的次序分配一个寄存器。
    /// 如果没有，则选取寄存器中最晚使用的寄存器，同时溢出寄存器到变量中
    /// @param var 分配寄存器的变量
    /// @param no 指定的寄存器编号
    /// @return int 寄存器编号
    ///
    int Allocate(Value * var = nullptr, int32_t no = -1);

    ///
    /// @brief 强制占用一个指定的寄存器。如果寄存器被占用，则强制寄存器关联的变量溢出
    /// @param no 要分配的寄存器编号
    ///
    void Allocate(int32_t no);

    ///
    /// @brief 将变量对应的load寄存器标记为空闲状态
    /// @param var 变量
    ///
    void free(Value * var);

    ///
    /// @brief 将寄存器no标记为空闲状态
    /// @param no 寄存器编号
    ///
    void free(int32_t);

protected:
    ///
    /// @brief 寄存器被置位，使用过的寄存器被置位
    /// @param no
    ///
    void bitmapSet(int32_t no);

protected:
    ///
    /// @brief 寄存器位图：1已被占用，0未被使用
    ///
    BitMap<PlatformArm32::maxUsableRegNum> regBitmap;

    ///
    /// @brief 寄存器被那个Value占用。按照时间次序加入
    ///
    std::vector<Value *> regValues;

    ///
    /// @brief 使用过的所有寄存器编号
    ///
    BitMap<PlatformArm32::maxUsableRegNum> usedBitmap;
};
