///
/// @file User.cpp
/// @brief 使用Value的User，该User也是Value。函数、指令都是User
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

#include "User.h"

///
/// @brief 构造函数
/// @param _type  类型
///
User::User(Type * _type) : Value(_type)
{}

///
/// @brief 更新指定Pos的Value
/// @param pos 位置
/// @param val 操作数
///
void User::setOperand(int32_t pos, Value * val)
{
    if (pos < (int32_t) uses.size()) {
        uses[pos]->setUsee(val);
    }
}

///
/// @brief 增加操作数，或者说本身的值由这些操作数来计算得到
/// @param pos 索引位置
/// @param val 值
///
void User::addOperand(Value * val)
{
    // If not, add the given Value as a new use.
    auto use = new Use(val, this);

    // 增加到操作数中
    uses.push_back(use);

    // 该val被使用
    val->addUse(use);
}

///
/// @brief 清除指定的操作数
/// @param val 操作数
///
void User::removeOperand(Value * val)
{
    for (auto & use: uses) {
        if (use->getUsee() == val) {
            // 找到了就删除这个Use
            use->remove();
            delete use;
            break;
        }
    }
}

///
/// @brief 清除指定的操作数
/// @param pos 操作数的索引
///
void User::removeOperand(int pos)
{
    // 检索并清除边，使得边的两头都会自动减少
    if (pos < (int32_t) uses.size()) {

        // 必须先暂存后释放，不能直接delete uses[pos]
        // 这是因为use->remove会删除operands的元素，使得operands[pos]的对象不再是原来的对象
        Use * use = uses[pos];
        use->remove();
        delete use;
    }
}

///
/// @brief 清除指定的操作数
/// @param pos 操作数的索引
///
void User::removeOperandRaw(Use * use)
{
    auto pIter = std::find(uses.begin(), uses.end(), use);
    if (pIter != uses.end()) {
        uses.erase(pIter);
    }
}

///
/// @brief 清除指定的Use
/// @param use define-use边
///
void User::removeUse(Use * use)
{
    auto pIter = std::find(uses.begin(), uses.end(), use);
    if (pIter != uses.end()) {
        use->remove();
    }
}

///
/// @brief 清除所有的操作数
///
void User::clearOperands()
{
    for (int32_t pos = 0; pos < (int32_t) uses.size();) {

        // 必须先暂存后释放，不能直接delete uses[pos]
        // 这是因为use->remove会删除operands的元素，使得operands[pos]的对象不再是原来的对象

        Use * use = uses[pos];
        use->remove();
        delete use;
    }
}
///
/// @brief Get the Operands object
/// @return std::vector<Use *>&
///
std::vector<Use *> & User::getOperands()
{
    return uses;
}

///
/// @brief 取得操作数
/// @return std::vector<Value *>
///
std::vector<Value *> User::getOperandsValue()
{
    std::vector<Value *> operandsVec;
    for (auto & use: uses) {
        operandsVec.emplace_back(use->getUsee());
    }
    return operandsVec;
}

///
/// @brief 获取操作数的个数
/// @return int32_t 个数
///
int32_t User::getOperandsNum()
{
    return (int32_t) uses.size();
}

///
/// @brief 获取指定的操作数
/// @param pos 位置
/// @return Value* 操作数
///
Value * User::getOperand(int32_t pos)
{
    if (pos < (int32_t) uses.size()) {
        return uses[pos]->getUsee();
    }

    return nullptr;
}
