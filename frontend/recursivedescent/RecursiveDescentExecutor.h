/**
 * @file RecursiveDescentExecutor.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 递归下降分析执行器的类原型
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */

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
