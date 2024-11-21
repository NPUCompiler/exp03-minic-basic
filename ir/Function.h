///
/// @file Function.cpp
/// @brief 函数头文件
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
#include <vector>

#include "GlobalValue.h"
#include "FunctionType.h"
#include "FormalParam.h"
#include "LocalVariable.h"
#include "MemVariable.h"
#include "IRCode.h"

///
/// @brief 描述函数信息的类，是全局静态存储，其Value的类型为FunctionType
///
class Function : public GlobalValue {

public:
    /// @brief 指定函数名字、函数返回类型以及函数形式参数的构造函数
    /// @param _name
    /// @param _type
    /// @param _param
    explicit Function(std::string _name, FunctionType * _type, bool _builtin = false);

    ///
    /// @brief 析构函数
    /// @brief 释放函数占用的内存和IR指令代码
    /// @brief 注意：IR指令代码并未释放，需要手动释放
    ~Function();

    /// @brief 获取函数返回类型
    /// @return 返回类型
    Type * getReturnType();

    /// @brief 获取函数的形参列表
    /// @return 形参列表
    std::vector<FormalParam *> & getParams();

    /// @brief 获取函数内的IR指令代码
    /// @return IR指令代码
    InterCode & getInterCode();

    /// @brief 判断该函数是否是内置函数
    /// @return true: 内置函数，false：用户自定义
    bool isBuiltin();

    /// @brief 函数指令信息输出
    /// @param str 函数指令
    void toString(std::string & str);

    /// @brief 设置函数出口指令
    /// @param inst 出口Label指令
    void setExitLabel(Instruction * inst);

    /// @brief 获取函数出口指令
    /// @return 出口Label指令
    Instruction * getExitLabel();

    /// @brief 设置函数返回值变量
    /// @param val 返回值变量，要求必须是局部变量，不能是临时变量
    void setReturnValue(LocalVariable * val);

    /// @brief 获取函数返回值变量
    /// @return 返回值变量
    LocalVariable * getReturnValue();

    /// @brief 获取函数内变量清单
    /// @return 函数内变量清单
    std::vector<LocalVariable *> & getVarValues()
    {
        return varsVector;
    }

    ///
    /// @brief  检查是否是函数
    /// @return true 是函数
    /// @return false 不是函数
    ///
    [[nodiscard]] bool isFunction() const override
    {
        return true;
    }

    /// @brief 获取最大栈帧深度
    /// @return 栈帧深度
    int getMaxDep();

    /// @brief 设置最大栈帧深度
    /// @param dep 栈帧深度
    void setMaxDep(int dep);

    /// @brief 获取函数调用栈空间大小而引入的栈空间大小
    /// @return 栈空间大小
    int getExtraStackSize();

    /// @brief 设置函数调用栈空间大小而引入的栈空间大小
    /// @param size 栈空间大小
    void setExtraStackSize(int size);

    /// @brief 获取函数调用参数个数的最大值
    /// @return 函数调用参数个数的最大值
    int getMaxFuncCallArgCnt();

    /// @brief 设置函数调用参数个数的最大值
    /// @param count 函数调用参数个数的最大值
    void setMaxFuncCallArgCnt(int count);

    /// @brief 函数内是否存在函数调用
    bool getExistFuncCall();

    /// @brief 设置函数是否存在函数调用
    /// @param exist true: 存在 false: 不存在
    void setExistFuncCall(bool exist);

    /// @brief 获取本函数需要保护的寄存器
    /// @return 要保护的寄存器
    std::vector<int32_t> & getProtectedReg();

    /// @brief 获取本函数需要保护的寄存器字符串
    /// @return 要保护的寄存器
    std::string & getProtectedRegStr();

    /// @brief 新建局部变量型Value
    /// @param name 变量ID
    /// @param type 变量类型
    /// @param existInit 缺省为true。若真，则已存在需要进行初始化，否则什么都不做
    LocalVariable * newLocalVarValue(Type * type, std::string name = "", int32_t scope_level = 1);

    /// @brief 新建一个内存型的Value，并加入到符号表，用于后续释放空间
    /// \param type 变量类型
    /// \return 临时变量Value
    MemVariable * newMemVariable(Type * type);

    /// @brief 清理函数内申请的资源
    void Delete();

    ///
    /// @brief 函数内的Value重命名，用于IR指令的输出
    ///
    void renameIR();

    ///
    /// @brief 获取统计的ARG指令的个数
    /// @return int32_t 个数
    ///
    int32_t getRealArgcount();

    ///
    /// @brief 用于统计ARG指令个数的自增函数，个数加1
    ///
    void realArgCountInc();

    ///
    /// @brief 用于统计ARG指令个数的清零
    ///
    void realArgCountReset();

private:
    ///
    /// @brief 函数的返回值类型，有点冗余，可删除，直接从type中取得即可
    ///
    Type * returnType;

    ///
    /// @brief 形式参数列表
    ///
    std::vector<FormalParam *> params;

    ///
    /// @brief 是否是内置函数或者外部库函数
    ///
    bool builtIn = false;

    ///
    /// @brief 线性IR指令块，可包含多条IR指令
    ///
    InterCode code;

    ///
    /// @brief 函数内变量的向量表，可能重名，请注意
    ///
    std::vector<LocalVariable *> varsVector;

    ///
    /// @brief 内存型Value
    ///
    std::vector<MemVariable *> memVector;

    ///
    /// @brief 函数出口Label指令
    ///
    Instruction * exitLabel = nullptr;

    ///
    /// @brief 函数返回值变量，不能是临时变量，必须是局部变量
    ///
    LocalVariable * returnValue = nullptr;

    ///
    /// @brief 由于局部变量、前4个形参需站内空间分配而导致的栈帧大小
    ///
    int maxDepth = 0;

    ///
    /// @brief 由于函数调用需要栈传递而导致的栈空间大小
    ///
    int maxExtraStackSize = 0;

    ///
    /// @brief 是否存在函数调用
    ///
    bool funcCallExist = false;

    ///
    /// @brief 本函数内函数调用的参数个数最大值
    ///
    int maxFuncCallArgCnt = 0;

    ///
    /// @brief 函数是否需要重定位，栈帧发生变化
    ///
    bool relocated = false;

    ///
    /// @brief 被保护的寄存器编号
    ///
    std::vector<int32_t> protectedRegs;

    ///
    /// @brief 被保护寄存器字符串
    ///
    std::string protectedRegStr;

    ///
    /// @brief 累计的实参个数，用于ARG指令的统计
    ///
    int32_t realArgCount = 0;
};
