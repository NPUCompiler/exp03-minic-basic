///
/// @file RecursiveDescentExecutor.cpp
/// @brief 递归下降分析执行器类的实现
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
#include "RecursiveDescentExecutor.h"
#include "RecursiveDescentFlex.h"
#include "RecursiveDescentParser.h"

/// @brief 前端词法与语法解析生成AST
/// @return true: 成功 false：错误
bool RecursiveDescentExecutor::run()
{
    // 若指定有参数，则作为词法分析的输入文件
    rd_filein = fopen(filename.c_str(), "r");
    if (rd_filein == nullptr) {
        printf("Can't open file %s\n", filename.c_str());
        return false;
    }

    // 如果要查看LALR的移进与归约过程，请设置yydebug为1
    // yydebug = 1;

    // 词法、语法分析生成抽象语法树AST
    astRoot = rd_parse();
    if (!astRoot) {

        // 关闭文件
        fclose(rd_filein);

        return false;
    }

    // 关闭文件
    fclose(rd_filein);

    return true;
}