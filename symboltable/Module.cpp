///
/// @file Module.cpp
/// @brief  符号表-模块类
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
#include "Module.h"

#include "ScopeStack.h"
#include "Common.h"
#include "VoidType.h"

Module::Module(std::string _name) : name(_name)
{
    // 创建作用域栈
    scopeStack = new ScopeStack();

    // 确保全局变量作用域入栈，这样全局变量才可以加入
    scopeStack->enterScope();

    // 加入内置函数putint
    (void) newFunction("putint", VoidType::getType(), {new FormalParam{IntegerType::getTypeInt(), ""}}, true);
    (void) newFunction("getint", IntegerType::getTypeInt(), {}, true);
}

/// @brief 进入作用域，如进入函数体块、语句块等
void Module::enterScope()
{
    scopeStack->enterScope();
}

/// @brief 退出作用域，如退出函数体块、语句块等
void Module::leaveScope()
{
    scopeStack->leaveScope();
}

///
/// @brief 在遍历抽象语法树的过程中，获取当前正在处理的函数。在函数外处理时返回空指针。
/// @return Function* 当前处理的函数对象
///
Function * Module::getCurrentFunction()
{
    return currentFunc;
}

///
/// @brief 设置当前正在处理的函数指针。函数外设置空指针
/// @param current 函数对象
///
void Module::setCurrentFunction(Function * current)
{
    currentFunc = current;
}

/// @brief 新建函数并放到函数列表中
/// @param name 函数名
/// @param returnType 返回值类型
/// @param params 形参列表
/// @param builtin 是否内置函数
/// @return 新建的函数对象实例
Function * Module::newFunction(std::string name, Type * returnType, std::vector<FormalParam *> params, bool builtin)
{
    // 先根据函数名查找函数，若找到则出错
    Function * tempFunc = findFunction(name);
    if (tempFunc) {
        // 函数已存在
        return nullptr;
    }

    // 根据形参创建形参类型清单
    std::vector<Type *> paramsType(params.size());

    for (auto & param: params) {
        paramsType.push_back(param->getType());
    }

    /// 函数类型参数
    FunctionType * type = new FunctionType(returnType, paramsType);

    // 新建函数对象
    tempFunc = new Function(name, type, builtin);

    // 设置参数
    tempFunc->getParams().assign(params.begin(), params.end());

    insertFunctionDirectly(tempFunc);

    return tempFunc;
}

/// @brief 根据函数名查找函数信息
/// @param name 函数名
/// @return 函数信息
Function * Module::findFunction(std::string name)
{
    // 根据名字查找
    auto pIter = funcMap.find(name);
    if (pIter != funcMap.end()) {
        // 查找到
        return pIter->second;
    }

    return nullptr;
}

///
/// @brief 直接向函数的符号表中加入函数。需外部检查函数的存在性
/// @param func 要加入的函数
///
void Module::insertFunctionDirectly(Function * func)
{
    funcMap.insert({func->getName(), func});
    funcVector.emplace_back(func);
}

/// @brief Value直接插入到符号表中的全局变量中
/// @param name Value的名称
/// @param val Value信息
void Module::insertGlobalValueDirectly(GlobalVariable * val)
{
    globalVariableMap.emplace(val->getName(), val);
    globalVariableVector.push_back(val);
}

/// @brief Value直接插入到符号表中的全局变量中
/// @param name Value的名称
/// @param val Value信息
void Module::insertConstIntDirectly(ConstInt * val)
{
    constIntMap.emplace(val->getVal(), val);
}

/// @brief 新建一个整型数值的Value，并加入到符号表，用于后续释放空间
/// @param intVal 整数值
/// @return 常量Value
ConstInt * Module::newConstInt(int32_t intVal)
{
    // 查找整数字符串
    ConstInt * val = findConstInt(intVal);
    if (!val) {

        // 不存在，则创建整数常量Value
        val = new ConstInt(intVal);

        insertConstIntDirectly(val);
    }

    return val;
}

/// @brief 根据整数值获取当前符号
/// \param name 变量名
/// \return 变量对应的值
ConstInt * Module::findConstInt(int32_t val)
{
    ConstInt * temp = nullptr;

    auto pIter = constIntMap.find(val);
    if (pIter != constIntMap.end()) {
        // 查找到
        temp = pIter->second;
    }

    return temp;
}

/// @brief 在当前的作用域中查找，若没有查找到则创建局部变量或者全局变量。请注意不能创建临时变量
/// ! 该函数只有在AST遍历生成线性IR中使用，其它地方不能使用
/// @param type 变量类型
/// @param name 变量ID 局部变量时可以为空，目的为了SSA时创建临时的局部变量，
/// @return nullptr则说明变量已存在，否则为新建的变量
Value * Module::newVarValue(Type * type, std::string name)
{
    Value * retVal;
    std::string varName;

    // 若变量名有效，检查当前作用域中是否存在变量，如存在则语义错误
    // 反之，因无效需创建新的变量名，肯定不现在的不同，不需要查找
    if (!name.empty()) {
        Value * tempValue = scopeStack->findCurrentScope(name);
        if (tempValue) {
            // 变量存在，语义错误
            minic_log(LOG_ERROR, "变量(%s)已经存在", name.c_str());
            return nullptr;
        }
    } else if (!currentFunc) {
        // 全局变量要求name不能为空串，必须有效
        minic_log(LOG_ERROR, "变量名为空");
        return nullptr;
    }

    if (currentFunc) {

        // 获取变量作用域的层级
        int32_t scope_level;
        if (name.empty()) {
            scope_level = 1;
        } else {
            scope_level = scopeStack->getCurrentScopeLevel();
        }

        retVal = currentFunc->newLocalVarValue(type, name, scope_level);

    } else {
        retVal = newGlobalVariable(type, name);
    }

    // 增加做作用域中
    scopeStack->insertValue(retVal);

    return retVal;
}

/// @brief 查找变量，会根据作用域栈进行逐级查找。
/// ! 该函数只有在AST遍历生成线性IR中使用，其它地方不能使用
///
/// @param name 变量ID
/// @return 指针有效则找到，空指针未找到
Value * Module::findVarValue(std::string name)
{
    // 逐层级作用域查找
    Value * tempValue = scopeStack->findAllScope(name);

    return tempValue;
}

///
/// @brief 新建全局变量，要求name必须有效，并且加入到全局符号表中。不检查是否现有的符号表中是否存在。
/// @param type 类型
/// @param name 名字
/// @return Value* 全局变量
///
GlobalVariable * Module::newGlobalVariable(Type * type, std::string name)
{
    GlobalVariable * val = new GlobalVariable(type, name);

    insertGlobalValueDirectly(val);

    return val;
}

/// @brief 根据变量名获取当前符号(只管理全局变量和常量)
/// @param name 变量名或者常量名
/// @param create 变量查找不到时若为true则自动创建变量型Value，否则不创建
/// @return 变量对应的值
GlobalVariable * Module::findGlobalVariable(std::string name)
{
    GlobalVariable * temp = nullptr;

    auto pIter = globalVariableMap.find(name);
    if (pIter != globalVariableMap.end()) {
        // 查找到
        temp = pIter->second;
    }

    return temp;
}

/// @brief 清理注册的所有Value资源
void Module::Delete()
{
    // 清除所有的函数
    for (auto func: funcVector) {
        delete func;
    }

    // 清理全局变量
    for (auto var: globalVariableVector) {
        delete var;
    }

    // 相关列表清空
    globalVariableMap.clear();
    globalVariableVector.clear();

    funcMap.clear();
    funcVector.clear();
}

///
/// @brief 对IR指令中没有名字的全部命名
///
void Module::renameIR()
{
    // 全局变量目前都有名字，目前不存在没有名字的变量，因此
    // 对于全局变量的线性IR名称，只是在原来的名称前追加@即可

    // 遍历所有的函数，含局部变量名、形参、Label名、指令变量重命名
    for (auto func: funcVector) {
        func->renameIR();
    }
}

/// @brief 文本输出线性IR指令
/// @param filePath 输出文件路径
void Module::outputIR(const std::string & filePath)
{
    // 这里使用C的文件操作，也可以使用C++的文件操作

    FILE * fp = fopen(filePath.c_str(), "w");
    if (nullptr == fp) {
        printf("fopen() failed\n");
        return;
    }

    // 全局变量遍历输出对应的declare指令
    for (auto var: globalVariableVector) {

        std::string str;
        var->toDeclareString(str);
        fprintf(fp, "%s\n", str.c_str());
    }

    // 遍历所有的线性IR指令，文本输出
    for (auto func: funcVector) {

        std::string instStr;
        func->toString(instStr);
        fprintf(fp, "%s", instStr.c_str());
    }

    fclose(fp);
}
