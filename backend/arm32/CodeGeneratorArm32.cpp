///
/// @file CodeGeneratorArm32.cpp
/// @brief ARM32的后端处理实现
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
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

#include "Function.h"
#include "Module.h"
#include "PlatformArm32.h"
#include "CodeGeneratorArm32.h"
#include "InstSelectorArm32.h"
#include "SimpleRegisterAllocator.h"
#include "ILocArm32.h"
#include "RegVariable.h"
#include "FuncCallInstruction.h"
#include "ArgInstruction.h"
#include "MoveInstruction.h"

/// @brief 构造函数
/// @param tab 符号表
CodeGeneratorArm32::CodeGeneratorArm32(Module * _module) : CodeGeneratorAsm(_module)
{}

/// @brief 析构函数
CodeGeneratorArm32::~CodeGeneratorArm32()
{}

/// @brief 产生汇编头部分
void CodeGeneratorArm32::genHeader()
{
    fprintf(fp, "%s\n", ".arch armv7ve");
    fprintf(fp, "%s\n", ".arm");
    fprintf(fp, "%s\n", ".fpu vfpv4");
}

/// @brief 全局变量Section，主要包含初始化的和未初始化过的
void CodeGeneratorArm32::genDataSection()
{
    // 生成代码段
    fprintf(fp, ".text\n");

    // 可直接操作文件指针fp进行写操作

    // 目前不支持全局变量和静态变量，以及字符串常量
    // 全局变量分两种情况：初始化的全局变量和未初始化的全局变量
    // TODO 这里先处理未初始化的全局变量
    for (auto var: module->getGlobalVariables()) {

        if (var->isInBSSSection()) {

            // 在BSS段的全局变量，可以包含初值全是0的变量
            fprintf(fp, ".comm %s, %d, %d\n", var->getName().c_str(), var->getType()->getSize(), var->getAlignment());
        } else {

            // 有初值的全局变量
            fprintf(fp, ".global %s\n", var->getName().c_str());
            fprintf(fp, ".data\n");
            fprintf(fp, ".align %d\n", var->getAlignment());
            fprintf(fp, ".type %s, %%object\n", var->getName().c_str());
            fprintf(fp, "%s\n", var->getName().c_str());
            // TODO 后面设置初始化的值，具体请参考ARM的汇编
        }
    }
}

///
/// @brief 获取IR变量相关信息字符串
/// @param str
///
void CodeGeneratorArm32::getIRValueStr(Value * val, std::string & str)
{
    std::string name = val->getName();
    std::string IRName = val->getIRName();
    int32_t regId = val->getRegId();
    int32_t baseRegId;
    int64_t offset;
    std::string showName;

    if (name.empty() && (!IRName.empty())) {
        showName = IRName;
    } else if ((!name.empty()) && IRName.empty()) {
        showName = IRName;
    } else if ((!name.empty()) && (!IRName.empty())) {
        showName = name + ":" + IRName;
    } else {
        showName = "";
    }

    if (regId != -1) {
        // 寄存器
        str += "\t@ " + showName + ":" + PlatformArm32::regName[regId];
    } else if (val->getMemoryAddr(&baseRegId, &offset)) {
        // 栈内寻址，[fp,#4]
        str += "\t@ " + showName + ":[" + PlatformArm32::regName[baseRegId] + ",#" + std::to_string(offset) + "]";
    }
}

/// @brief 针对函数进行汇编指令生成，放到.text代码段中
/// @param func 要处理的函数
void CodeGeneratorArm32::genCodeSection(Function * func)
{
    // 寄存器分配以及栈内局部变量的站内地址重新分配
    registerAllocation(func);

    // 获取函数的指令列表
    std::vector<Instruction *> & IrInsts = func->getInterCode().getInsts();

    // 汇编指令输出前要确保Label的名字有效，必须是程序级别的唯一，而不是函数内的唯一。要全局编号。
    for (auto inst: IrInsts) {
        if (inst->getOp() == IRInstOperator::IRINST_OP_LABEL) {
            inst->setName(IR_LABEL_PREFIX + std::to_string(labelIndex++));
        }
    }

    // ILOC代码序列
    ILocArm32 iloc(module);

    // 指令选择生成汇编指令
    InstSelectorArm32 instSelector(IrInsts, iloc, func, simpleRegisterAllocator);
    instSelector.setShowLinearIR(this->showLinearIR);
    instSelector.run();

    // 删除无用的Label指令
    iloc.deleteUnusedLabel();

    // ILOC代码输出为汇编代码
    fprintf(fp, ".align %d\n", func->getAlignment());
    fprintf(fp, ".global %s\n", func->getName().c_str());
    fprintf(fp, ".type %s, %%function\n", func->getName().c_str());
    fprintf(fp, "%s:\n", func->getName().c_str());

    // 开启时输出IR指令作为注释
    if (this->showLinearIR) {

        // 输出有关局部变量的注释，便于查找问题
        for (auto localVar: func->getVarValues()) {
            std::string str;
            getIRValueStr(localVar, str);
            if (!str.empty()) {
                fprintf(fp, "%s\n", str.c_str());
            }
        }

        // 输出指令关联的临时变量信息
        for (auto inst: func->getInterCode().getInsts()) {
            if (inst->hasResultValue()) {
                std::string str;
                getIRValueStr(inst, str);
                if (!str.empty()) {
                    fprintf(fp, "%s\n", str.c_str());
                }
            }
        }
    }

    iloc.outPut(fp);
}

/// @brief 寄存器分配
/// @param func 函数指针
void CodeGeneratorArm32::registerAllocation(Function * func)
{
    // 内置函数不需要处理
    if (func->isBuiltin()) {
        return;
    }

    // 最简单/朴素的寄存器分配简单，但性能差，具体如下：
    // (1) 局部变量都保存在内存栈中（含简单变量、下标变量等）
    // (2) 全局变量在静态存储.data区中
    // (3) 指令类的临时变量也保存在内存栈中，但是性能很差

    // ARM32的函数调用约定：
    // R0,R1,R2和R3寄存器不需要保护，可直接使用
    // SP寄存器预留，不需要保护，但需要保证值的正确性
    // R4-R10, fp(11), lx(14)都需要保护，没有函数调用的函数可不用保护lx寄存器
    // 被保留的寄存器主要有：
    //  (1) FP寄存器用于栈寻址，即R11
    //  (2) LX寄存器用于函数调用，即R14。没有函数调用的函数可不用保护lx寄存器
    //  (3) R10寄存器用于立即数过大时要通过寄存器寻址，这里简化处理进行预留

    // 至少有FP和LX寄存器需要保护
    std::vector<int32_t> & protectedRegNo = func->getProtectedReg();
    protectedRegNo.clear();
    protectedRegNo.push_back(ARM32_TMP_REG_NO);
    protectedRegNo.push_back(ARM32_FP_REG_NO);
    if (func->getExistFuncCall()) {
        protectedRegNo.push_back(ARM32_LX_REG_NO);
    }

    // 调整函数调用指令，主要是前四个寄存器传值，后面用栈传递
    // 为了更好的进行寄存器分配，可以进行对函数调用的指令进行预处理
    // 当然也可以不做处理，不过性能更差。这个处理是可选的。
    adjustFuncCallInsts(func);

    // 为局部变量和临时变量在栈内分配空间，指定偏移，进行栈空间的分配
    stackAlloc(func);

    // 函数形参要求前四个寄存器分配，后面的参数采用栈传递，实现实参的值传递给形参
    // 这一步是必须的
    adjustFormalParamInsts(func);

#if 0
    // 临时输出调整后的IR指令，用于查看当前的寄存器分配、栈内变量分配、实参入栈等信息的正确性
    std::string irCodeStr;
    func->renameIR();
    func->toString(irCodeStr);
    std::cout << irCodeStr << std::endl;
#endif
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorArm32::adjustFormalParamInsts(Function * func)
{
    // 函数形参的前四个实参值采用的是寄存器传值，后面栈传递

    auto & params = func->getParams();

    // 形参的前四个通过寄存器来传值R0-R3
    for (int k = 0; k < (int) params.size() && k <= 3; k++) {

        // 前四个设置分配寄存器
        params[k]->setRegId(k);
    }

    // 根据ARM版C语言的调用约定，除前4个外的实参进行值传递，逆序入栈
    int64_t fp_esp = func->getProtectedReg().size() * 4;
    for (int k = 4; k < (int) params.size(); k++) {

        params[k]->setMemoryAddr(ARM32_FP_REG_NO, fp_esp);

        // 增加4字节，目前只支持int类型
        fp_esp += params[k]->getType()->getSize();
    }
}

/// @brief 寄存器分配前对函数内的指令进行调整，以便方便寄存器分配
/// @param func 要处理的函数
void CodeGeneratorArm32::adjustFuncCallInsts(Function * func)
{
    std::vector<Instruction *> newInsts;

    // 当前函数的指令列表
    auto & insts = func->getInterCode().getInsts();

    // 函数返回值用R0寄存器，若函数调用有返回值，则赋值R0到对应寄存器
    // 通过栈传递的实参，采用SP + 偏移的方式殉职，偏移肯定非负。
    for (auto pIter = insts.begin(); pIter != insts.end(); pIter++) {

        // 检查是否是函数调用指令，并且含有返回值
        if (Instanceof(callInst, FuncCallInstruction *, *pIter)) {

            // 实参前四个要寄存器传值，其它参数通过栈传递

            int32_t argNum = callInst->getOperandsNum();

            // 除前四个整数寄存器外，后面的参数采用栈传递
            int esp = 0;
            for (int32_t k = 4; k < argNum; k++) {

                // 获取实参的值
                auto arg = callInst->getOperand(k);

                // 栈帧空间（低地址在前，高地址在后）
                // --------------------- sp
                // 实参栈传递的空间（排除寄存器传递的实参空间）
                // ---------------------
                // 需要保存在栈中的局部变量或临时变量或形参对应变量空间
                // --------------------- fp
                // 保护寄存器的空间
                // ---------------------

                // 新建一个内存变量，把实参的值保存到栈中，以便栈传值，其寻址为SP + 非负偏移
                MemVariable * newVal = func->newMemVariable(IntegerType::getTypeInt());
                newVal->setMemoryAddr(ARM32_SP_REG_NO, esp);
                esp += 4;

                // 引入赋值指令，把实参的值保存到内存变量上
                Instruction * assignInst = new MoveInstruction(func, newVal, arg);

                // 更换实参变量为内存变量
                callInst->setOperand(k, newVal);

                // 赋值指令插入到函数调用指令的前面
                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

            // ARM32的函数调用约定，前四个参数通过寄存器传递
            for (int k = 0; k < argNum && k < 4; k++) {

                // 把实参的值通过move指令传递给寄存器

                auto arg = callInst->getOperand(k);

                Instruction * assignInst = new MoveInstruction(func, PlatformArm32::intRegVal[k], arg);

                callInst->setOperand(k, PlatformArm32::intRegVal[k]);

                // 函数调用指令前插入后，pIter仍指向函数调用指令
                pIter = insts.insert(pIter, assignInst);
                pIter++;
            }

#if 0
            for (int k = 0; k < callInst->getOperandsNum(); k++) {

                auto arg = callInst->getOperand(k);

                // 产生ARG指令
                pIter = insts.insert(pIter, new ArgInstruction(func, arg));
                pIter++;
            }
#endif

            // 有arg指令后可不用参数，展示不删除
            // args.clear();

            // 赋值指令
            if (callInst->hasResultValue()) {

                if (callInst->getRegId() == 0) {
                    // 结果变量的寄存器和返回值寄存器一样，则什么都不需要做
                    ;
                } else {
                    // 其它情况，需要产生赋值指令
                    // 新建一个赋值操作
                    Instruction * assignInst = new MoveInstruction(func, callInst, PlatformArm32::intRegVal[0]);

                    // 函数调用指令的下一个指令的前面插入指令，因为有Exit指令，+1肯定有效
                    pIter = insts.insert(pIter + 1, assignInst);
                }
            }
        }
    }
}

/// @brief 栈空间分配
/// @param func 要处理的函数
void CodeGeneratorArm32::stackAlloc(Function * func)
{
    // 栈内分配的空间除了寄存器保护所分配的空间之外，还需要管理如下的空间
    // (1) 没有指派寄存器的局部变量、形参或临时变量的栈内分配
    // (2) 函数调用时需要栈内传递的实参
    // (3) 函数内定义的数组变量需要在栈内分配
    // (4) 函数内定义的静态变量空间分配按静态分配处理

    // 遍历函数内的所有指令，查找没有寄存器分配的变量，然后进行栈内空间分配

    // 栈帧空间
    // --------------------- sp
    // 实参栈传递的空间（排除寄存器传递的实参空间）
    // ---------------------
    // 需要保存在栈中的局部变量或临时变量或形参对应变量空间
    // --------------------- fp
    // 保护寄存器的空间
    // ---------------------

    // 这里对临时变量和局部变量都在栈上进行分配，采用FP+偏移的寻址方式，偏移为负数

    int32_t sp_esp = 0;

    // 遍历函数变量列表
    for (auto var: func->getVarValues()) {

        // 对于简单类型的寄存器分配策略，假定临时变量和局部变量都保存在栈中，属于内存
        // 而对于图着色等，临时变量一般是寄存器，局部变量也可能修改为寄存器
        // TODO 考虑如何进行分配使得临时变量尽量保存在寄存器中，作为优化点考虑

        // regId不为-1，则说明该变量分配为寄存器
        // baseRegNo不等于-1，则说明该变量肯定在栈上，属于内存变量，之前肯定已经分配过
        if ((var->getRegId() == -1) && (!var->getMemoryAddr())) {

            // 该变量没有分配寄存器

            int32_t size = var->getType()->getSize();

            // 32位ARM平台按照4字节的大小整数倍分配局部变量
            size = (size + 3) & ~3;

            // 累计当前作用域大小
            sp_esp += size;

            // 这里要注意检查变量栈的偏移范围。一般采用机制寄存器+立即数方式间接寻址
            // 若立即数满足要求，可采用基址寄存器+立即数变量的方式访问变量
            // 否则，需要先把偏移量放到寄存器中，然后机制寄存器+偏移寄存器来寻址
            // 之后需要对所有使用到该Value的指令在寄存器分配前要变换。

            // 局部变量偏移设置
            var->setMemoryAddr(ARM32_FP_REG_NO, -sp_esp);
        }
    }

    // 遍历包含有值的指令，也就是临时变量
    for (auto inst: func->getInterCode().getInsts()) {

        if (inst->hasResultValue() && (inst->getRegId() == -1)) {
            // 有值，并且没有分配寄存器

            int32_t size = inst->getType()->getSize();

            // 32位ARM平台按照4字节的大小整数倍分配局部变量
            size = (size + 3) & ~3;

            // 累计当前作用域大小
            sp_esp += size;

            // 这里要注意检查变量栈的偏移范围。一般采用机制寄存器+立即数方式间接寻址
            // 若立即数满足要求，可采用基址寄存器+立即数变量的方式访问变量
            // 否则，需要先把偏移量放到寄存器中，然后机制寄存器+偏移寄存器来寻址
            // 之后需要对所有使用到该Value的指令在寄存器分配前要变换。

            // 局部变量偏移设置
            inst->setMemoryAddr(ARM32_FP_REG_NO, -sp_esp);
        }
    }

    // 通过栈传递的实参，ARM32的前四个通过寄存器传递
    int maxFuncCallArgCnt = func->getMaxFuncCallArgCnt();
    if (maxFuncCallArgCnt > 4) {
        sp_esp += (maxFuncCallArgCnt - 4) * 4;
    }

    // 只有int类型时可以4字节对齐，支持浮点或者向量运算时要16字节对齐
    // sp_esp = (sp_esp + 15) & ~15;

    // 设置函数的最大栈帧深度，没有考虑寄存器保护的空间大小
    func->setMaxDep(sp_esp);
}
