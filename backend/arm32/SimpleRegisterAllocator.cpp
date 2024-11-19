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
#include "SimpleRegisterAllocator.h"

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
int SimpleRegisterAllocator::Allocate(Value * var, int32_t no)
{
    if (var && (var->getLoadRegId() != -1)) {
        // 该变量已经分配了Load寄存器了，不需要再次分配
        return var->getLoadRegId();
    }

    int32_t regno = -1;

    // 尝试指定的寄存器是否可用
    if ((no != -1) && !regBitmap.test(no)) {

        // 可用
        regno = no;
    } else {

        // 查询空闲的寄存器
        for (int k = 0; k < PlatformArm32::maxUsableRegNum; ++k) {

            if (!regBitmap.test(k)) {

                // 找到空闲寄存器
                regno = k;
                break;
            }
        }
    }

    if (regno != -1) {

        // 找到空闲的寄存器

        // 占用
        bitmapSet(regno);

    } else {

        // 没有可用的寄存器分配，需要溢出一个变量的寄存器

        // 溢出的策略：选择最迟加入队列的变量
        Value * oldestVar = regValues.front();

        // 获取Load寄存器编号，设置该变量不再占用Load寄存器
        regno = oldestVar->getLoadRegId();

        // 设置该变量不再占用寄存器
        oldestVar->setLoadRegId(-1);

        // 从队列中删除
        regValues.erase(regValues.begin());
    }

    if (var) {
        // 加入新的变量
        var->setLoadRegId(regno);
        regValues.push_back(var);
    }

    return regno;
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
    if (var && (var->getLoadRegId() != -1)) {

        // 清除该索引的寄存器，变得可使用
        regBitmap.reset(var->getLoadRegId());
        regValues.erase(std::find(regValues.begin(), regValues.end(), var));
        var->setLoadRegId(-1);
    }
}

///
/// @brief 将寄存器no标记为空闲状态
/// @param no 寄存器编号
///
void SimpleRegisterAllocator::free(int32_t no)
{
    // 无效寄存器，什么都不做，直接返回
    if (no == -1) {
        return;
    }

    // 清除该索引的寄存器，变得可使用
    regBitmap.reset(no);

    // 查找寄存器编号
    auto pIter = std::find_if(regValues.begin(), regValues.end(), [=](auto val) {
        return val->getLoadRegId() == no; // 存器编号与 no 匹配
    });

    if (pIter != regValues.end()) {
        // 查找到，则清除，并设置为-1
        (*pIter)->setLoadRegId(-1);
        regValues.erase(pIter);
    }
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