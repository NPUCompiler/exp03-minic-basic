///
/// @file Type.h
/// @brief 类型的基类，描述所有的类型
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

#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

class Type {

public:
    ///
    /// @brief 类型ID
    ///
    enum TypeID {
        // PrimitiveTypes
        FloatTyID, ///< 32-bit floating point type
        VoidTyID,  ///< type with no size
        LabelTyID, ///< Labels
        TokenTyID, ///< Tokens

        // Derived types
        IntegerTyID,  ///< Arbitrary bit width integers
        FunctionTyID, ///< Functions
        PointerTyID,  ///< Pointers
        ArrayTyID,    ///< Arrays
    };

    ///
    /// @brief 构造函数
    /// @param _ID 类型ID
    ///
    Type(TypeID _ID = VoidTyID) : ID(_ID)
    {}

    ///
    /// @brief 缺省的析构函数
    ///
    virtual ~Type() = default;

    ///
    /// @brief 下列操作不被允许，只能通过指针传递
    ///
    Type(const Type &) = delete;
    Type(Type &&) = delete;
    Type & operator=(const Type &) = delete;
    Type & operator=(Type &&) = delete;

    ///
    /// @brief 检查是否是VOID类型
    /// @return true 是
    /// @return false  不是
    ///
    [[nodiscard]] bool isVoidType() const
    {
        return ID == VoidTyID;
    }

    ///
    /// @brief 检查是否是Label类型
    /// @return true 是
    /// @return false  不是
    ///
    [[nodiscard]] bool isLabelType() const
    {
        return ID == LabelTyID;
    }

    ///
    /// @brief 检查是否是函数类型
    /// @return true 是
    /// @return false  不是
    ///
    [[nodiscard]] bool isFunctionType() const
    {
        return ID == FunctionTyID;
    }

    ///
    /// @brief 检查是否是整数类型
    /// @return true 是
    /// @return false  不是
    ///
    [[nodiscard]] bool isIntegerType() const
    {
        return ID == IntegerTyID;
    }

    ///
    /// @brief 检查是否是单精度浮点型类型
    /// @return true 是
    /// @return false  不是
    ///
    [[nodiscard]] bool isFloatType() const
    {
        return ID == FloatTyID;
    }

    ///
    /// @brief 是否是布尔类型，也就是1位整数类型
    /// @return true
    /// @return false
    ///
    [[nodiscard]] virtual bool isInt1Byte() const
    {
        return false;
    }

    ///
    /// @brief 是否是int类型，也就是32位整数类型
    /// @return true
    /// @return false
    ///
    [[nodiscard]] virtual bool isInt32Type() const
    {
        return false;
    }

    ///
    /// @brief 是否是指针类型
    /// @return true 是
    /// @return false 不是
    ///
    [[nodiscard]] bool isPointerType() const
    {
        return ID == PointerTyID;
    }

    ///
    /// @brief 是否是数组类型
    /// @return true 是
    /// @return false 不是
    ///
    [[nodiscard]] bool isArrayType() const
    {
        return ID == ArrayTyID;
    }

    ///
    /// @brief 获取类型ID
    /// @return TypeID
    ///
    [[nodiscard]] TypeID getTypeID() const
    {
        return ID;
    }

    ///
    /// @brief 获得类型所占内存空间大小
    /// @return int32_t
    ///
    [[nodiscard]] virtual int32_t getSize() const
    {
        return -1;
    }

    /// @brief 转换字符串
    /// @return 字符串
    [[nodiscard]] virtual std::string toString() const = 0;

protected:
    ///
    /// @brief 标识类型的ID
    ///
    TypeID ID;
};
