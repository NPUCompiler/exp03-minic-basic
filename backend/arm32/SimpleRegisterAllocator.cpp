///
/// @file SimpleRegisterAllocator.cpp
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
#include <algorithm>
#include <cassert>
#include "SimpleRegisterAllocator.h"
#include "Value.h"

///
/// @brief Construct a new Simple Register Allocator object
///
SimpleRegisterAllocator::SimpleRegisterAllocator()
{}

///
/// @brief 分配一个寄存器。如果没有，则选取寄存器中最晚使用的寄存器，同时溢出寄存器到变量中
/// @return int 寄存器编号
/// @param no 指定的寄存器编号
///
std::pair<int32_t, Value *> SimpleRegisterAllocator::Allocate(Value * var, int32_t no)
{
    int32_t regno = -1;
    Value * spillValue = nullptr;

    // 指定要分配的寄存器，则检查是否被占用；若占用则先溢出后分配，否则直接分配
    // 如果没有指定寄存器，则从小到大分配一个寄存器
    if (no != -1) {

        // 指定的Value已经分配了该寄存器，直接返回即可
        if (var && (var->getRegId() == no)) {
            return {no, spillValue};
        }

        // 指定的寄存器没有被占用，则直接分配
        // 若被占用，则必须要先溢出占用该寄存器的变量，再次分配
        if (!regBitmap.test(no)) {

            regno = no;
        }

    } else {
        //  从小到大分配一个寄存器
        for (int k = 0; k < PlatformArm32::maxUsableRegNum; ++k) {
            if (!regBitmap.test(k)) {
                regno = k;
                break;
            }
        }
    }

    if (regno == -1) {

        // 没有可用的寄存器分配，需要溢出一个变量的寄存器

        // 查找占用该寄存器的变量，该变量需要溢出
        if (no != -1) {
            // 看指定的寄存器编号被那个Value占用
            spillValue = free(no);
        } else {
            // 释放最早使用寄存器的变量进行溢出
            spillValue = free(regValues[0]->getRegId());
        }

        if (!spillValue) {
            // 肯定有溢出的Value，但这里没有，异常
            assert(false && "No available register to spill");
            return {-1, spillValue};
        }

        // 获取到寄存器编号
        regno = spillValue->getRegId();

        // 保存溢出的Value
        spillValues.push_back(spillValue);
    }

    if (var) {
        // 分配寄存器给指定的Value
        var->setRegId(regno);
        regValues.push_back(var);
    }

    // 设置寄存器被占用
    bitmapSet(regno);

    return {regno, spillValue};
}

///
/// @brief 强制占用一个指定的寄存器。如果寄存器被占用，则强制寄存器关联的变量溢出
/// @param no 要分配的寄存器编号
///
void SimpleRegisterAllocator::Allocate(int32_t no)
{
    if (regBitmap.test(no)) {

        // 指定的寄存器已经被占用

        // 释放该寄存器
        free(no);
    }

    // 占用该寄存器
    bitmapSet(no);
}

///
/// @brief 将变量对应的load寄存器标记为空闲状态
/// @param var 变量
///
void SimpleRegisterAllocator::free(Value * var)
{
    // 清除该索引的寄存器，变得可使用
    free(var->getRegId());
}

///
/// @brief 将寄存器no标记为空闲状态
/// @param no 寄存器编号
///
Value * SimpleRegisterAllocator::free(int32_t no)
{
    Value * freeValue = nullptr;

    // 无效寄存器，什么都不做，直接返回
    if (no < 0) {
        return nullptr;
    }

    // 清除该索引的寄存器，变得可使用
    regBitmap.reset(no);

    // 查找寄存器编号
    auto pIter = std::find_if(regValues.begin(), regValues.end(), [=](Value * val) {
        return val->getRegId() == no; // 存器编号与 no 匹配
    });

    if (pIter != regValues.end()) {
        // 查找到，则清除，并设置为-1
        freeValue = *pIter;
        freeValue->setRegId(-1);
        regValues.erase(pIter);
    }

    return freeValue;
}

///
/// @brief 寄存器被置位，使用过的寄存器被置位
/// @param no
///
void SimpleRegisterAllocator::bitmapSet(int32_t no)
{
    regBitmap.set(no);
    usedBitmap.set(no);
}