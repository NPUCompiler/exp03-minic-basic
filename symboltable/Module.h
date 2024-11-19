///
/// @file Module.h
/// @brief 符号表-模块类
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

#include <string>
#include <vector>
#include <unordered_map>

#include "ConstInt.h"
#include "Type.h"
#include "GlobalVariable.h"
#include "Function.h"

class ScopeStack;

///
/// @brief  一个Module代表一个C语言的源文件
///
class Module {

public:
    ///
    /// @brief 构造函数
    /// @param name  模块名
    ///
    Module(std::string _name);

    ///
    /// @brief 缺省的析构函数
    ///
    virtual ~Module() = default;

    ///
    /// @brief 输出IR代码
    /// @return std::string IR代码字符串
    ///
    std::string toIRString();

    ///
    /// @brief 根据后端输出汇编代码
    /// @return std::string 汇编代码字符串
    ///
    // 获取模块的名字
    [[nodiscard]] std::string getName() const
    {
        return name;
    }

    /// @brief 进入作用域，如进入函数体块、语句块等
    void enterScope();

    /// @brief 退出作用域，如退出函数体块、语句块等
    void leaveScope();

    ///
    /// @brief 在遍历抽象语法树的过程中，获取当前正在处理的函数。在函数外处理时返回空指针。
    ///
    Function * getCurrentFunction();

    ///
    /// @brief 设置当前正在处理的函数指针。函数外设置空指针
    /// @param current
    ///
    void setCurrentFunction(Function * current);

    /// @brief 新建函数并放到函数列表中
    /// @param name 函数名
    /// @param returnType 返回值类型
    /// @param params 形参列表
    /// @param builtin 是否内置函数
    /// @return 新建的函数对象实例
    Function *
    newFunction(std::string name, Type * returnType, std::vector<FormalParam *> params = {}, bool builtin = false);

    /// @brief 根据函数名查找函数信息
    /// @param name 函数名
    /// @return 函数信息
    Function * findFunction(std::string name);

    ///
    /// @brief 获取全局变量列表，用于外部遍历全局变量
    /// @return std::vector<GlobalVariable *>&
    ///
    std::vector<GlobalVariable *> & getGlobalVariables()
    {
        return globalVariableVector;
    }

    /// @brief 获得函数列表
    std::vector<Function *> & getFunctionList()
    {
        return funcVector;
    }

    /// @brief 新建一个整型数值的Value，并加入到符号表，用于后续释放空间
    /// \param intVal 整数值
    /// \return 临时Value
    ConstInt * newConstInt(int32_t intVal);

    /// @brief 新建变量型Value，会根据currentFunc的值进行判断创建全局或者局部变量
    /// ! 该函数只有在AST遍历生成线性IR中使用，其它地方不能使用
    /// @param name 变量ID
    /// @param type 变量类型
    Value * newVarValue(Type * type, std::string name = "");

    /// @brief 查找变量（全局变量或局部变量），会根据作用域栈进行逐级查找。
    /// ! 该函数只有在AST遍历生成线性IR中使用，其它地方不能使用
    /// @param name 变量ID
    /// @return 指针有效则找到，空指针未找到
    Value * findVarValue(std::string name);

    /// @brief 清理Module中管理的所有信息资源
    void Delete();

    /// @brief 输出线性IR指令列表
    /// @param filePath
    void outputIR(const std::string & filePath);

    ///
    /// @brief 对IR指令中没有名字的全部命名
    ///
    void renameIR();

protected:
    /// @brief 根据整数值获取当前符号
    /// \param name 变量名
    /// \return 变量对应的值
    ConstInt * findConstInt(int32_t val);

    ///
    /// @brief 新建全局变量，要求name必须有效，并且加入到全局符号表中。
    /// @param type 类型
    /// @param name 名字
    /// @return Value* 全局变量
    ///
    GlobalVariable * newGlobalVariable(Type * type, std::string name);

    /// @brief 根据变量名获取当前符号（只管理全局变量）
    /// \param name 变量名
    /// \return 变量对应的值
    GlobalVariable * findGlobalVariable(std::string name);

    /// @brief 直接插入函数到符号表中，不考虑现有的表中是否存在
    /// @param func 函数对象
    void insertFunctionDirectly(Function * func);

    /// @brief Value插入到符号表中
    /// @param val Value信息
    void insertGlobalValueDirectly(GlobalVariable * val);

    /// @brief ConstInt插入到符号表中
    /// @param val Value信息
    void insertConstIntDirectly(ConstInt * val);

private:
    ///
    /// @brief 模块名，也就是要编译的文件名
    ///
    std::string name;

    ///
    /// @brief 所有的类型，便于内存的释放
    ///
    std::vector<Type *> types;

    /// @brief  变量作用域栈
    ScopeStack * scopeStack;

    /// @brief 遍历抽象树过程中的当前处理函数
    Function * currentFunc = nullptr;

    /// @brief 函数映射表，函数名-函数，便于检索
    std::unordered_map<std::string, Function *> funcMap;

    /// @brief  函数列表
    std::vector<Function *> funcVector;

    /// @brief 变量名映射表，变量名-变量，只保存全局变量
    std::unordered_map<std::string, GlobalVariable *> globalVariableMap;

    /// @brief 只保存全局变量
    std::vector<GlobalVariable *> globalVariableVector;

    /// @brief 常量表
    std::unordered_map<int32_t, ConstInt *> constIntMap;
};