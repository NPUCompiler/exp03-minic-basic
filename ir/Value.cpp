///
/// @file Value.cpp
/// @brief 值操作类型，所有的变量、函数、常量都是Value
///
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

#include "Value.h"
#include "Use.h"

/// @brief 构造函数
/// @param _type
Value::Value(Type * _type) : type(_type)
{
    // 不需要增加代码
}

/// @brief 析构函数
Value::~Value()
{
    // 如有资源清理，请这里追加代码
}

/// @brief 获取名字
/// @return 变量名
std::string Value::getName() const
{
    return name;
}

///
/// @brief 设置名字
/// @param _name 名字
///
void Value::setName(std::string _name)
{
    this->name = _name;
}

/// @brief 获取名字
/// @return 变量名
std::string Value::getIRName() const
{
    return IRName;
}

///
/// @brief 设置名字
/// @param _name 名字
///
void Value::setIRName(std::string _name)
{
    this->IRName = _name;
}

/// @brief 获取类型
/// @return 变量名
Type * Value::getType()
{
    return type;
}

///
/// @brief 增加一条边，增加Value被使用次数
/// @param use
///
void Value::addUse(Use * use)
{
    uses.push_back(use);
}

///
/// @brief 消除一条边，减少Value被使用次数
/// @param use
///
void Value::removeUse(Use * use)
{
    auto pIter = std::find(uses.begin(), uses.end(), use);
    if (pIter != uses.end()) {
        uses.erase(pIter);
    }
}

///
/// @brief 取得变量所在的作用域层级
/// @return int32_t 层级
///
int32_t Value::getScopeLevel()
{
    return -1;
}

///
/// @brief 获得分配的寄存器编号或ID
/// @return int32_t 寄存器编号 -1代表无效的寄存器编号
///
int32_t Value::getRegId()
{
    return -1;
}

///
/// @brief @brief 如是内存变量型Value，则获取基址寄存器和偏移
/// @param regId 寄存器编号
/// @param offset 相对偏移
/// @return true 是内存型变量
/// @return false 不是内存型变量
///
bool Value::getMemoryAddr(int32_t * regId, int64_t * offset)
{
    (void) regId;
    (void) offset;
    return false;
}

///
/// @brief 对该Value进行Load用的寄存器编号
/// @return int32_t 寄存器编号
///
int32_t Value::getLoadRegId()
{
    return -1;
}

///
/// @brief 对该Value进行Load用的寄存器编号
/// @return int32_t 寄存器编号
///
void Value::setLoadRegId(int32_t regId)
{
    (void) regId;
}