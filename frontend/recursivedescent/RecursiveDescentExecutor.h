///
/// @file RecursiveDescentExecutor.h
/// @brief 递归下降分析执行器类的头文件
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
#include "FrontEndExecutor.h"

/// @brief 归下降分析执行器类
class RecursiveDescentExecutor : public FrontEndExecutor {
public:
    /// @brief 构造函数
    /// @param filename 输入的源文件
    RecursiveDescentExecutor(std::string filename) : FrontEndExecutor(filename)
    {}

    /// @brief 析构函数
    ~RecursiveDescentExecutor() override
    {}

    /// @brief 前端词法与语法解析生成AST
    /// @return true: 成功 false：错误
    bool run() override;
};
