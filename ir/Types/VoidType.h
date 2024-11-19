///
/// @file VoidType.h
/// @brief void类型描述类
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

#include "Type.h"

class VoidType : public Type {

public:
    ///
    /// @brief 获取类型，全局只有一份
    /// @return VoidType*
    ///
    static VoidType * getType();

    ///
    /// @brief 获取类型的IR标识符
    /// @return std::string IR标识符void
    ///
    [[nodiscard]] std::string toString() const override
    {
        return "void";
    }

private:
    ///
    /// @brief 构造函数
    ///
    explicit VoidType() : Type(Type::VoidTyID)
    {}

    ///
    /// @brief 唯一的VOID类型实例
    ///
    static VoidType * oneInstance;
};
