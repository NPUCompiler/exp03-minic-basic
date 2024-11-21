///
/// @file ILocArm32.h
/// @brief 指令序列管理的头文件，ILOC的全称为Intermediate Language for Optimizing Compilers
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

#include <list>
#include <string>

#include "Instruction.h"
#include "Module.h"

#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

/// @brief 底层汇编指令：ARM32
struct ArmInst {

    /// @brief 操作码
    std::string opcode;

    /// @brief 条件
    std::string cond;

    /// @brief 结果
    std::string result;

    /// @brief 源操作数1
    std::string arg1;

    /// @brief 源操作数2
    std::string arg2;

    /// @brief 附加信息
    std::string addition;

    /// @brief 标识指令是否无效
    bool dead;

    /// @brief 构造函数
    /// @param op
    /// @param rs
    /// @param s1
    /// @param s2
    /// @param add
    ArmInst(std::string op,
            std::string rs = "",
            std::string s1 = "",
            std::string s2 = "",
            std::string cond = "",
            std::string extra = "");

    /// @brief 指令更新
    /// @param op
    /// @param rs
    /// @param s1
    /// @param s2
    /// @param add
    void replace(std::string op,
                 std::string rs = "",
                 std::string s1 = "",
                 std::string s2 = "",
                 std::string cond = "",
                 std::string extra = "");

    /// @brief 设置死指令
    void setDead();

    /// @brief 指令字符串输出函数
    /// @return
    std::string outPut();
};

/// @brief 底层汇编序列-ARM32
class ILocArm32 {

    /// @brief ARM汇编序列
    std::list<ArmInst *> code;

    /// @brief 符号表
    Module * module;

    /// @brief 加载立即数 ldr r0,=#100
    /// @param rs_reg_no 结果寄存器号
    /// @param num 立即数
    void load_imm(int rs_reg_no, int num);

    /// @brief 加载符号值 ldr r0,=g; ldr r0,[r0]
    /// @param rsReg 结果寄存器号
    /// @param name Label名字
    void load_symbol(int rs_reg_no, std::string name);

    /// @brief 加载栈内变量地址
    /// @param rsReg 结果寄存器号
    /// @param base_reg_no 基址寄存器
    /// @param off 偏移
    void leaStack(int rs_reg_no, int base_reg_no, int offset);

public:
    /// @brief 构造函数
    /// @param _module 符号表-模块
    ILocArm32(Module * _module);

    /// @brief 析构函数
    ~ILocArm32();

    ///
    /// @brief 注释指令，不包含分号
    /// @param str 注释内容
    ///
    void comment(std::string str);

    /// @brief 数字变字符串，若flag为真，则变为立即数寻址（加#）
    /// @param num 立即数
    /// @param flag 是否家#
    /// @return 字符串
    std::string toStr(int num, bool flag = true);

    /// @brief 获取当前的代码序列
    /// @return 代码序列
    std::list<ArmInst *> & getCode();

    /// @brief Load指令，基址寻址 ldr r0,[fp,#100]
    /// @param rs_reg_no 结果寄存器
    /// @param base_reg_no 基址寄存器
    /// @param disp 偏移
    void load_base(int rs_reg_no, int base_reg_no, int disp);

    /// @brief Store指令，基址寻址 str r0,[fp,#100]
    /// @param src_reg_no 源寄存器
    /// @param base_reg_no 基址寄存器
    /// @param disp 偏移
    /// @param tmp_reg_no 可能需要临时寄存器编号
    void store_base(int src_reg_no, int base_reg_no, int disp, int tmp_reg_no);

    /// @brief 标签指令
    /// @param name
    void label(std::string name);

    /// @brief 一个操作数指令
    /// @param op 操作码
    /// @param rs 操作数
    void inst(std::string op, std::string rs);

    /// @brief 一个操作数指令
    /// @param op 操作码
    /// @param rs 操作数
    /// @param arg1 源操作数
    void inst(std::string op, std::string rs, std::string arg1);

    /// @brief 一个操作数指令
    /// @param op 操作码
    /// @param rs 操作数
    /// @param arg1 源操作数
    /// @param arg2 源操作数
    void inst(std::string op, std::string rs, std::string arg1, std::string arg2);

    /// @brief 加载变量到寄存器
    /// @param rs_reg_no 结果寄存器
    /// @param var 变量
    void load_var(int rs_reg_no, Value * var);

    /// @brief 加载变量地址到寄存器
    /// @param rs_reg_no 结果寄存器
    /// @param var 变量
    void lea_var(int rs_reg_no, Value * var);

    /// @brief 保存寄存器到变量
    /// @param src_reg_no 源寄存器号
    /// @param var 变量
    /// @param addr_reg_no 地址寄存器号
    void store_var(int src_reg_no, Value * var, int addr_reg_no);

    /// @brief 寄存器Mov操作
    /// @param rs_reg_no 结果寄存器
    /// @param src_reg_no 源寄存器
    void mov_reg(int rs_reg_no, int src_reg_no);

    /// @brief 调用函数fun
    /// @param fun
    void call_fun(std::string name);

    /// @brief 分配栈帧
    /// @param func 函数
    /// @param tmp_reg_No
    void allocStack(Function * func, int tmp_reg_No);

    /// @brief 加载函数的参数到寄存器
    /// @param fun
    void ldr_args(Function * fun);

    /// @brief NOP操作
    void nop();

    ///
    /// @brief 无条件跳转指令
    /// @param label 目标Label名称
    ///
    void jump(std::string label);

    /// @brief 输出汇编
    /// @param file 输出的文件指针
    /// @param outputEmpty 是否输出空语句
    void outPut(FILE * file, bool outputEmpty = false);

    /// @brief 删除无用的Label指令
    void deleteUsedLabel();
};
