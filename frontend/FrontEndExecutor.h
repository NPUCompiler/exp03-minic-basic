///
/// @file FrontEndExecutor.h
/// @brief 前端分析执行器的接口类
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

#include "AST.h"

///
/// @brief 前端执行器的接口类
///
class FrontEndExecutor {
public:
    ///
    /// @brief 构造函数
    /// @param[in] 源文件路径
    ///
    FrontEndExecutor(std::string _filename) : filename(_filename)
    {}

    ///
    /// @brief 析构函数
    ///
    virtual ~FrontEndExecutor()
    {}

    /// @brief 前端执行器的运行函数
    /// @return true: 成功 false: 失败
    virtual bool run() = 0;

    ///
    /// @brief  返回抽象语法树的根
    /// @return ast_node*
    ///
    ast_node * getASTRoot()
    {
        return astRoot;
    }

protected:
    ///
    /// @brief 要解析的文件路径
    ///
    std::string filename;

    ///
    /// @brief  抽象语法树的根
    ///
    ast_node * astRoot;
};
