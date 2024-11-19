/**
 * @file RecursiveDescentFlex.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 词法分析的手动实现源文件
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include "RecursiveDescentParser.h"
#include "Common.h"

/// @brief 词法分析的行号信息
int64_t rd_line_no = 1;

/// @brief 词法分析的token对应的字符识别
std::string ctex;

/// @brief 输入源文件指针
FILE * rd_filein;

/// @brief 关键字与Token类别的数据结构
struct KeywordToken {
    std::string name;
    enum RDTokenType type;
};

/// @brief  关键字与Token对应表
static KeywordToken allKeywords[] = {
    {"int", RDTokenType::T_INT},
    {"return", RDTokenType::T_RETURN},
};

/// @brief 在标识符中检查是否时关键字，若是关键字则返回对应关键字的Token，否则返回T_ID
/// @param id 标识符
/// @return Token
static RDTokenType getKeywordToken(std::string id)
{
    //如果在allkeywords中找到，则说明为关键字
    for (auto & keyword: allKeywords) {
        if (keyword.name == id) {
            return keyword.type;
        }
    }
	// 如果不再allkeywords中，说明是标识符
    return RDTokenType::T_ID;
}

/// @brief 词法文法，获取下一个Token
/// @return  Token，值保存在rd_lval中
int rd_flex()
{
    int c;

    //忽略空格
    while ((c = fgetc(rd_filein)) == ' ' || c == '\t' || c == '\n') {

        // TODO 请支持Linux/Windows/Mac系统的行号分析
        if (c == '\n') {
            rd_line_no++;
        }
    }

    // 文件结束符
    if (c == EOF) {
        //返回文件结束符
        return RDTokenType::T_EOF;
    }

    // TODO 请自行实现删除源文件中的注释，含单行注释和多行注释等

    // 处理数字
    if (isdigit(c)) {

        // 识别无符号数，这里只处理正整数或者0
        // 0开头的整数这里也识别成了10进制整数，请注意

        rd_lval.integer_num.lineno = rd_line_no;
        rd_lval.integer_num.val = c - '0';

        // 最长匹配，直到非数字结束
        while (isdigit(c = fgetc(rd_filein))) {
            rd_lval.integer_num.val = rd_lval.integer_num.val * 10 + c - '0';
        }
        // 存储数字的token值
        ctex = std::to_string(rd_lval.integer_num.val);
        // 多读的字符回退
        ungetc(c, rd_filein);

        c = RDTokenType::T_DIGIT;
    } else if (c == '+') {
        // 识别字符+
        c = RDTokenType::T_ADD;
        // 存储字符+
        ctex = "+";
    } else if (c == '-') {
        // 识别字符-
        c = RDTokenType::T_SUB;
        // 存储字符-
        ctex = "-";
    } else if (c == '(') {
        // 识别字符(
        c = RDTokenType::T_L_PAREN;
        // 存储字符(
        ctex = "(";
    } else if (c == ')') {
        // 识别字符)
        c = RDTokenType::T_R_PAREN;
        // 存储字符)
        ctex = ")";
    } else if (c == '{') {
        // 识别字符{
        c = RDTokenType::T_L_BRACE;
        // 存储字符{
        ctex = "{";
    } else if (c == '}') {
        // 识别字符}
        c = RDTokenType::T_R_BRACE;
        // 存储字符}
        ctex = "}";
    } else if (c == '=') {
        // 识别字符=
        c = RDTokenType::T_ASSIGN;
        // 存储字符=
        ctex = "=";
    } else if (c == ';') {
        // 识别字符;
        c = RDTokenType::T_SEMICOLON;
        // 存储字符;
        ctex = ";";
    } else if (c == ',') {
        // 识别字符,
        c = RDTokenType::T_COMMA;
        // 存储字符,
        ctex = ",";
    } else if (isLetterUnderLine(c)) {
        // 识别标识符，包含关键字/保留字或自定义标识符

        // 最长匹配标识符
        std::string name;

        do {
            // 记录字符
            name.push_back(c);
            c = fgetc(rd_filein);
        } while (isLetterDigitalUnderLine(c));
        // 存储标识符
        ctex = name;
        // 多读的字符恢复，下次可继续读到该字符
        ungetc(c, rd_filein);

        // 检查是否是关键字，若是则返回对应的Token，否则返回T_ID
        c = getKeywordToken(name);
        if (c == RDTokenType::T_ID) {

            // 设置ID的值
            rd_lval.var_id.id = strdup(name.c_str());

            // 设置行号
            rd_lval.var_id.lineno = rd_line_no;
        } else if (c == RDTokenType::T_INT) {
            // 设置ID的值
            rd_lval.type.type = BasicType::TYPE_INT;

            // 设置行号
            rd_lval.type.lineno = rd_line_no;
        }
    } else {
        printf("Line(%lld): Invalid char %s\n", (long long) rd_line_no, ctex.c_str());
        c = RDTokenType::T_ERR;
    }

    /* Return a single char. */
    return c;
}
