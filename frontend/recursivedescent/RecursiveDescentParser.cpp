///
/// @file RecursiveDescentParser.cpp
/// @brief 递归下降分析法实现的语法分析后产生抽象语法树的实现
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
#include <stdarg.h>

#include "AST.h"
#include "AttrType.h"
#include "RecursiveDescentFlex.h"
#include "RecursiveDescentParser.h"

// 定义全局变量给词法分析使用，用于填充值
RDSType rd_lval;

// 词法识别的记号值，原始文本字符串
extern std::string tokenValue;

// 语法分析过程中的错误数目
static int errno_num = 0;

// 语法分析过程中的LookAhead，指向下一个Token
static RDTokenType lookaheadTag = RDTokenType::T_EMPTY;

///
/// @brief 继续检查LookAhead指向的记号是否是T，用于符号的FIRST集合或Follow集合判断
///
#define _(T) || (lookaheadTag == T)

///
/// @brief 第一个检查LookAhead指向的记号是否属于C，用于符号的FIRST集合或Follow集合判断
/// 如判断是否是T_ID，或者T_INT，可结合F和_两个宏来实现，即F(T_ID) _(T_INT)
///
#define F(C) (lookaheadTag == C)

///
/// @brief lookahead指向下一个Token
///
static void advance()
{
    lookaheadTag = (RDTokenType) rd_flex();
}

///
/// @brief flag若匹配则跳过Token，使得LookAhead指向下一个Token
/// @param tag 是否匹配指定的Tag
/// @return true：匹配，false：未匹配
///
static bool match(RDTokenType tag)
{
    bool result = false;

    if (F(tag)) {

        result = true;

        // 匹配，则向前获取下一个Token
        advance();
    }

    return result;
}

///
/// @brief 语法错误输出
/// @param format 格式化字符串，和printf的格式化字符串一样
///
static void semerror(const char * format, ...)
{
    char logStr[1024];

    va_list ap;
    va_start(ap, format);

    // 利用vsnprintf函数将可变参数按照一定的格式，格式化为一个字符串。
    vsnprintf(logStr, sizeof(logStr), format, ap);

    va_end(ap);

    printf("Line(%d): %s\n", rd_line_no, logStr);

    errno_num++;
}

///
/// @brief 表达式文法 expr : T_DIGIT的识别，目前只识别无符号整数
/// @return AST的节点
///
static ast_node * expr()
{
    ast_node * node = nullptr;
    if (F(T_DIGIT)) {
        // 无符号整数

        node = ast_node::New(rd_lval.integer_num);

        // 跳过当前记号，指向下一个记号
        advance();
    }

    return node;
}

///
/// @brief 语句的识别，其文法为：statement -> T_RETURN expr T_SEMICOLON
/// @return AST的节点
///
static ast_node * statement()
{

    if (match(T_RETURN)) {

        // return语句的First集合元素为T_RETURN
        // 若匹配，则说明是return语句

        ast_node * expr_node = expr();

        if (!match(T_SEMICOLON)) {

            // 返回语句后没有分号
            semerror("返回语句后没有分号");
        }

        return create_contain_node(ast_operator_type::AST_OP_RETURN, expr_node);
    }

    return nullptr;
}

///
/// @brief 块中的项目识别，其文法为：blockItem -> statement
/// @return AST的节点
///
static ast_node * BlockItem()
{
    return statement();
}

///
/// @brief 块内语句列表识别，文法为BlockItemList : BlockItem+
/// @return AST的节点
///
static void BlockItemList(ast_node * blockNode)
{
    for (;;) {

        // 如果是右大括号，则结束循环，提升效率
        if (F(T_R_BRACE)) {
            break;
        }

        // 遍历BlockItem
        ast_node * itemNode = BlockItem();
        if (itemNode) {
            blockNode->insert_son_node(itemNode);
        } else {
            // 没有，则结束
            break;
        }
    }
}

///
/// @brief 语句块识别，文法：Block -> T_L_BRACE BlockItemList? T_R_BRACE
/// @return AST的节点
///
static ast_node * Block()
{
    if (match(T_L_BRACE)) {

        // 创建语句块节点
        ast_node * blockNode = create_contain_node(ast_operator_type::AST_OP_BLOCK);

        // 空的语句块
        if (match(T_R_BRACE)) {
            return blockNode;
        }

        // 块内语句列表识别
        BlockItemList(blockNode);

        // 没有匹配左大括号，则语法错误
        if (!match(T_R_BRACE)) {
            semerror("缺少右大括号");
        }

        // 正常
        return blockNode;
    }

    // 语法解析失败
    return nullptr;
}

///
/// @brief 函数定义的识别，其文法为： funcDef -> T_INT T_ID T_L_PAREN T_R_PAREN block
/// @return ast_node 函数运算符节点
///
static ast_node * funcDef()
{
    if (F(T_INT)) {

        // 函数返回之后类型
        type_attr funcReturnType = rd_lval.type;

        // 跳过当前的记号，指向下一个记号
        advance();

        // 检测是否是标识符
        if (F(T_ID)) {

            // 获取标识符的值和定位信息
            var_id_attr funcId = rd_lval.var_id;

            // 跳过当前的记号，指向下一个记号
            advance();

            // 函数定义的左右括号识别
            if (match(T_L_PAREN)) {
                // 函数定义

                // 目前函数定义没有形参，因此必须是右小括号
                if (match(T_R_PAREN)) {

                    // 识别block
                    ast_node * blockNode = Block();

                    // 形参结点没有，设置为空指针
                    ast_node * formalParamsNode = nullptr;

                    // 创建函数定义的节点，孩子有类型，函数名，语句块和形参(实际上无)
                    // create_func_def函数内会释放funcId中指向的标识符空间，切记，之后不要再释放，之前一定要是通过strdup函数或者malloc分配的空间
                    return create_func_def(funcReturnType, funcId, blockNode, formalParamsNode);

                } else {
                    semerror("函数定义缺少右小括号");
                }

            } else {

                semerror("函数定义缺少左小括号");
            }
        } else {
            semerror("函数返回值类型后缺少函数名标识符");
        }
    }

    return nullptr;
}

///
/// @brief 编译单元识别（C语言文件），其文法为：compileUnit -> funcDef EOF
/// compileUnit是文法的开始符号
/// @return ast_node* 抽象语法树根节点
///
static ast_node * compileUnit()
{
    // 创建AST的根节点，编译单元运算符
    ast_node * cu_node = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT);

    // funcDef的First集合为{T_INT}，根据LL(1)文法可知若LookAhead记号为T_INT，则是函数定义
    if (F(T_INT)) {

        ast_node * node = funcDef();

        // 加入到父节点中，node为空时insert_son_node内部进行了忽略
        (void) cu_node->insert_son_node(node);
    }

    if (!match(T_EOF)) {
        // 没有文件结束
        semerror("文件结束标记不符");
    }

    return cu_node;
}

///
/// @brief 采用递归下降分析法实现词法与语法分析生成抽象语法树
/// @return ast_node* 空指针失败，否则成功
///
ast_node * rd_parse()
{
    // 没有错误信息
    errno_num = 0;

    // lookahead指向第一个Token
    advance();

    ast_node * astRoot = compileUnit();

    // 如果有错误信息，则返回-1，否则返回0
    if (errno_num != 0) {
        return nullptr;
    }

    return astRoot;
}
