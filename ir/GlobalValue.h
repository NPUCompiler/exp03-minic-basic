///
/// @file GlobalValue.h
/// @brief 描述全局值或对象的类，可以是常量、函数、全局变量等
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

#include "Constant.h"
#include "IRConstant.h"

///
/// @brief 代表全局对象，都是常量。
///
/// ! 全局变量的地址和函数的地址一般链接完成后都是常量。
/// ! 全局变量的IR名称是真实名称的前面加@符号
///
/// GlobalValue 主要用于表示全局作用域的可识别实体，
/// 而 Constant 主要用于表示可在编译期求值的常量。
/// 之所以 GlobalValue 需要作为一个独立的类，主要有以下几个原因：
/// 1. GlobalValue 代表的是全局对象，而 Constant 只是值。
/// GlobalValue 主要用于表示全局作用域中的命名实体，它包括：GlobalVariable（全局变量）和Function（函数）等
/// 这些对象都是全局可见的符号，可以被链接器识别，并可能参与链接。
/// 相比之下，Constant 只是一个值，它可能用于 GlobalValue 的初始化，但本身不是独立的全局实体。
/// 例如把3赋值给全局变量a，a就是一个GlobalValue，而3是一个Constant。
/// 2. GlobalValue 需要管理链接属性（Linkage），而 Constant 不需要。
/// 全局变量和函数通常涉及链接（Linkage）和可见性（Visibility），而普通的 Constant 不需要这些属性。
/// 3. GlobalValue 可能是可变的。Constant 表示的是永远不会改变的值，但 GlobalValue（尤其是 GlobalVariable）可能是可变的。
/// 4. GlobalValue 需要支持不同的存储模型。如静态存储、线程本地存储（TLS）、动态库导入/导出等。
/// 5. 方便优化器和代码生成。普通常量（可以在编译期内联，不占用全局存储），GlobalVariable 可能需要加载它的地址才能访问它的值。
/// GlobalValue 可能需要地址加载，而 Constant 可以直接内联
///
class GlobalValue : public Constant {

    ///
    /// @brief 用于区分函数或变量是否是static，或者外部都可见
    ///
    enum LinkageTypes {
        ExternalLinkage = 0, ///< Externally visible function
        InternalLinkage,     ///< Rename collisions when linking (static functions).
    };

    /// 全局对象的作用域，可见，或者只对文件可见，也就是隐藏的
    enum VisibilityTypes {
        DefaultVisibility = 0, ///< The GlobalValue is visible
        HiddenVisibility,      ///< The GlobalValue is hidden
        ProtectedVisibility    ///< The GlobalValue is protected
    };

public:
    ///
    /// @brief 构造函数
    /// @param _type  类型
    /// @param _name  全局符号名
    ///
    GlobalValue(Type * _type, std::string _name) : Constant(_type)
    {
        this->name = _name;
        this->IRName = IR_GLOBAL_VARNAME_PREFIX + this->name;
    }

    /// @brief 获取名字
    /// @return 变量名
    [[nodiscard]] std::string getIRName() const override
    {
        return IRName;
    }

    ///
    /// @brief  检查是否是函数
    /// @return true 是函数
    /// @return false 不是函数
    ///
    [[nodiscard]] virtual bool isFunction() const
    {
        return false;
    }

    ///
    /// @brief  检查是否是函数
    /// @return true 是函数
    /// @return false 不是函数
    ///
    [[nodiscard]] virtual bool isGlobalVarible() const
    {
        return false;
    }

    ///
    /// @brief 获得Linkage状态
    /// @return LinkageTypes
    ///
    [[nodiscard]] LinkageTypes getLinkage() const
    {
        return linkage;
    }

    ///
    /// @brief 获得作用域相关的可见度
    /// @return visibility
    ///
    [[nodiscard]] VisibilityTypes getVisibility() const
    {
        return visibility;
    }

    ///
    /// @brief 获取对齐大小
    /// @return int32_t 对齐大小
    ///
    [[nodiscard]] int32_t getAlignment() const
    {
        return alignment;
    }

    ///
    /// @brief 设置对齐大小
    /// @param _alignment
    ///
    void setAlignment(int32_t _alignment)
    {
        this->alignment = _alignment;
    }

protected:
    ///
    /// @brief The linkage of this global
    ///
    LinkageTypes linkage;

    ///
    /// @brief The visibility style of this global
    ///
    VisibilityTypes visibility;

    ///
    /// @brief 默认对齐大小为4字节
    ///
    int32_t alignment = 4;
};