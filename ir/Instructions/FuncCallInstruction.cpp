///
/// @file FuncCallInstruction.cpp
/// @brief 函数调用指令
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
#include "FuncCallInstruction.h"
#include "Function.h"
#include "Common.h"
#include "Type.h"

/// @brief 含有参数的函数调用
/// @param srcVal 函数的实参Value
/// @param result 保存返回值的Value
FuncCallInstruction::FuncCallInstruction(Function * _func,
                                         Function * calledFunc,
                                         std::vector<Value *> & _srcVal,
                                         Type * _type)
    : Instruction(_func, IRInstOperator::IRINST_OP_FUNC_CALL, _type), calledFunction(calledFunc)
{
    name = calledFunc->getName();

    // 实参拷贝
    for (auto & val: _srcVal) {
        addOperand(val);
    }
}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void FuncCallInstruction::toString(std::string & str)
{
    int32_t argCount = func->getRealArgcount();
    int32_t operandsNum = getOperandsNum();

    if (operandsNum != argCount) {
        // 两者不一致 也可能没有ARG指令，正常
        if (argCount != 0) {
            minic_log(LOG_ERROR, "ARG指令的个数与调用函数个数不一致");
        }
    }

    // TODO 这里应该根据函数名查找函数定义或者声明获取函数的类型
    // 这里假定所有函数返回类型要么是i32，要么是void
    // 函数参数的类型是i32

    if (type->isVoidType()) {

        // 函数没有返回值设置
        str = "call void " + calledFunction->getIRName() + "(";
    } else {

        // 函数有返回值要设置到结果变量中
        str = type->toString() + " " + getIRName() + " = call i32 " + calledFunction->getIRName() + "(";
    }

    if (argCount == 0) {

        // 如果没有arg指令，则输出函数的实参
        for (int32_t k = 0; k < operandsNum; ++k) {

            auto operand = getOperand(k);

            str += operand->getType()->toString() + " " + operand->getIRName();

            if (k != (operandsNum - 1)) {
                str += ", ";
            }
        }
    }

    str += ")";

    // 要清零
    func->realArgCountReset();
}

///
/// @brief 获取被调用函数的名字
/// @return std::string 被调用函数名字
///
std::string FuncCallInstruction::getCalledName() const
{
    return calledFunction->getName();
}
