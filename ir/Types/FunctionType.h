///
/// @file FunctionType.h
/// @brief 函数类型类，主要有函数返回值类型以及各个形参类型组成
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

#include <string>
#include <vector>

#include "Type.h"

class FunctionType final : public Type {
public:
    ///
    /// @brief 函数类型
    /// @param retType 函数返回值类型
    /// @param argTypes 函数形参类型
    ///
    FunctionType(Type * _retType, std::vector<Type *> _argTypes) : retType{_retType}, argTypes{std::move(_argTypes)}
    {}

    ///
    /// @brief 函数类型的IR字符串
    /// @return std::string
    ///
    [[nodiscard]] std::string toString() const override
    {
        std::string typeStr;

        typeStr = retType->toString() + " (*)(";

        // 遍历形参类型
        bool first = true;
        for (Type * type: argTypes) {

            if (first) {
                typeStr += type->toString();
                first = false;
            } else {
                typeStr += ", " + type->toString();
            }
        }

        typeStr += ")";

        return typeStr;
    }

    ///
    /// @brief 获取返回类型
    /// @return const Type*
    ///
    [[nodiscard]] Type * getReturnType() const
    {
        return retType;
    }

    ///
    /// @brief 获取形参清单
    /// @return const std::vector<const Type *>&
    ///
    [[nodiscard]] const std::vector<Type *> & getArgTypes() const
    {
        return argTypes;
    }

private:
    ///
    /// @brief 返回类型
    ///
    Type * retType;

    ///
    /// @brief 形参类型清单
    ///
    std::vector<Type *> argTypes;
};
