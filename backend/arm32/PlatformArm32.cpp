///
/// @file PlatformArm32.cpp
/// @brief  ARM32平台相关实现
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
#include "PlatformArm32.h"

#include "IntegerType.h"

const std::string PlatformArm32::regName[PlatformArm32::maxRegNum] = {
    "r0",  // 用于传参或返回值等，不需要栈保护
    "r1",  // 用于传参或返回值（64位结果时后32位）等，不需要栈保护
    "r2",  // 用于传参等，不需要栈保护
    "r3",  // 用于传参等，不需要栈保护
    "r4",  // 需要栈保护
    "r5",  // 需要栈保护
    "r6",  // 需要栈保护
    "r7",  // 需要栈保护
    "r8",  // 用于加载操作数1,保存表达式结果
    "r9",  // 用于加载操作数2,写回表达式结果,立即数，标签地址
    "r10", // 用于保存乘法结果，虽然mul
           // r8,r8,r9也能正常执行，但是避免交叉编译提示错误！
    "fp",  // r11,局部变量寻址
    "ip",  // r12，临时寄存器
    "sp",  // r13，堆栈指针寄存器
    "lr", // r14，链接寄存器。LR存储子程序调用的返回地址。当执行BL指令时，PC的当前值会被保存到LR中。
    "pc", // r15，程序计数器。PC 存储着下一条将要执行的指令的地址。在执行分支指令时，PC会更新为新的地址。
};

RegVariable * PlatformArm32::intRegVal[PlatformArm32::maxRegNum] = {
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[0], 0),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[1], 1),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[2], 2),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[3], 3),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[4], 4),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[5], 5),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[6], 6),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[7], 7),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[8], 8),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[9], 9),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[10], 10),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[11], 11),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[12], 12),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[13], 13),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[14], 14),
    new RegVariable(IntegerType::getTypeInt(), PlatformArm32::regName[15], 15),
};

/// @brief 循环左移两位
/// @param num
void PlatformArm32::roundLeftShiftTwoBit(unsigned int & num)
{
    // 取左移即将溢出的两位
    const unsigned int overFlow = num & 0xc0000000;

    // 将溢出部分追加到尾部
    num = (num << 2) | (overFlow >> 30);
}

/// @brief 判断num是否是常数表达式，8位数字循环右移偶数位得到
/// @param num
/// @return
bool PlatformArm32::__constExpr(int num)
{
    unsigned int new_num = (unsigned int) num;

    for (int i = 0; i < 16; i++) {

        if (new_num <= 0xff) {
            // 有效表达式
            return true;
        }

        // 循环左移2位
        roundLeftShiftTwoBit(new_num);
    }

    return false;
}

/// @brief 同时处理正数和负数
/// @param num
/// @return
bool PlatformArm32::constExpr(int num)
{
    return __constExpr(num) || __constExpr(-num);
}

/// @brief 判定是否是合法的偏移
/// @param num
/// @return
bool PlatformArm32::isDisp(int num)
{
    return num < 4096 && num > -4096;
}

/// @brief 判断是否是合法的寄存器名
/// @param s 寄存器名字
/// @return 是否是
bool PlatformArm32::isReg(std::string name)
{
    return name == "r0" || name == "r1" || name == "r2" || name == "r3" || name == "r4" || name == "r5" ||
           name == "r6" || name == "r7" || name == "r8" || name == "r9" || name == "r10" || name == "fp" ||
           name == "ip" || name == "sp" || name == "lr" || name == "pc";
}
