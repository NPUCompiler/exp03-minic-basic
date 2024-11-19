///
/// @file FormalParam.h
/// @brief 函数形参描述类
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
#pragma once

#include "Value.h"
#include "IRConstant.h"

/// @brief 描述函数形参类
class FormalParam : public Value {

public:
    /// @brief 基本类型的参数
    /// @param _name 形参的名字
    /// @param _type 基本类型
    FormalParam(Type * _type, std::string _name) : Value(_type)
    {
        this->name = _name;
    };

    // /// @brief 输出字符串
    // /// @param str
    // std::string toString() override
    // {
    //     return type->toString() + " " + IRName;
    // }

    ///
    /// @brief 获得分配的寄存器编号或ID
    /// @return int32_t 寄存器编号
    ///
    int32_t getRegId() override
    {
        return regId;
    }

    ///
    /// @brief @brief 如是内存变量型Value，则获取基址寄存器和偏移
    /// @param regId 寄存器编号
    /// @param offset 相对偏移
    /// @return true 是内存型变量
    /// @return false 不是内存型变量
    ///
    bool getMemoryAddr(int32_t * _regId = nullptr, int64_t * _offset = nullptr) override
    {
        if (this->baseRegNo == -1) {
            return false;
        }

        if (_regId) {
            *_regId = this->baseRegNo;
        }
        if (_offset) {
            *_offset = this->offset;
        }

        return true;
    }

    ///
    /// @brief 设置内存寻址的基址寄存器和偏移
    /// @param _regId 基址寄存器编号
    /// @param _offset 偏移
    ///
    void setMemoryAddr(int32_t _regId, int64_t _offset)
    {
        baseRegNo = _regId;
        offset = _offset;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    int32_t getLoadRegId() override
    {
        return this->loadRegNo;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    void setLoadRegId(int32_t regId) override
    {
        this->loadRegNo = regId;
    }

    ///
    /// @brief 设置寄存器编号
    /// @param _regId 寄存器编号
    ///
    void setRegId(int32_t _regId)
    {
        this->regId = _regId;
    }

private:
    ///
    /// @brief 寄存器编号，-1表示没有分配寄存器，大于等于0代表寄存器分配后用寄存器寻址
    ///
    int32_t regId = -1;

    ///
    /// @brief 变量在栈内的偏移量，对于全局变量默认为0，临时变量没有意义
    ///
    int32_t offset = 0;

    ///
    /// @brief 栈内寻找时基址寄存器编号
    ///
    int32_t baseRegNo = -1;

    ///
    /// @brief 栈内寻找时基址寄存器名字
    ///
    std::string baseRegName;

    ///
    /// @brief 变量加载到寄存器中时对应的寄存器编号
    ///
    int32_t loadRegNo = -1;
};
