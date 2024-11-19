///
/// @file Antlr4Executor.cpp
/// @brief antlr4的词法与语法分析解析器
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
#include <iostream>

#include "AST.h"
#include "Antlr4Executor.h"
#include "Antlr4CSTVisitor.h"
#include "MiniCLexer.h"
#include "Common.h"

/// @brief 前端词法与语法解析生成AST
/// @return true: 成功 false：错误
bool Antlr4Executor::run()
{
    std::ifstream ifs;
    ifs.open(filename);
    if (!ifs.is_open()) {
        minic_log(LOG_ERROR, "文件(%s)不能打开，可能不存在", filename.c_str());
        return false;
    }

    // antlr4的输入流类实例
    antlr4::ANTLRInputStream input{ifs};

    // 词法分析器实例
    MiniCLexer lexer{&input};

    // 词法分析器实例转化成记号(Token)流
    antlr4::CommonTokenStream tokenStream{&lexer};

    // 利用antlr4进行分析，从compileUnit开始分析输入字符串
    MiniCParser parser{&tokenStream};

    // 从具体语法树的根结点进行深度优先遍历，生成抽象语法树
    auto cstRoot = parser.compileUnit();
    if (!cstRoot) {
        minic_log(LOG_ERROR, "Antlr4的词语与语法分析错误");
        return false;
    }

    /// 新建遍历器对具体语法树进行分析，产生抽象语法树
    MiniCCSTVisitor visitor;

    // 遍历产生抽象语法树
    astRoot = visitor.run(cstRoot);

    return true;
}
