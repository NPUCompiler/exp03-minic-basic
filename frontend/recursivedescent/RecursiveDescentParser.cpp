/**
 * @file RecursiveDescentParser.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 递归下降分析法实现的语法分析后产生抽象语法树的实现
 * @version 0.1
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdarg.h>

#include "AST.h"
#include "AttrType.h"
#include "RecursiveDescentFlex.h"
#include "RecursiveDescentParser.h"

// 定义全局变量给词法分析使用，用于填充值
RDSType rd_lval;

// 语法分析过程中的错误数目
static int errno_num = 0;

// 语法分析过程中的LookAhead，指向下一个Token
static RDTokenType lookaheadTag = RDTokenType::T_EMPTY;

// 定义两个宏，用于判断是否是对应的Token
#define _(T) || (lookaheadTag == T)
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

/// @brief 表达式文法 expr : T_DIGIT
/// @return AST的节点
static ast_node * expr()
{
    if (F(T_DIGIT)) {
        // 无符号整数

        ast_node * node = ast_node::New(rd_lval.integer_num);

        // 跳过当前记号，指向下一个记号
        advance();

        return node;
    }

    return nullptr;
}

/// @brief returnStatement -> T_RETURN expr T_SEMICOLON
/// @return AST的节点
static ast_node * returnStatement()
{
    if (match(T_RETURN)) {

        // 匹配成功return关键字并移动到下一个字符

        ast_node * expr_node = expr();

        if (!match(T_SEMICOLON)) {
            // 返回语句后没有分号
            semerror("返回语句后没有分号");
        }

        return ast_node::New(ast_operator_type::AST_OP_RETURN, expr_node, nullptr);
    }

    // 语法失败返回空节点，不可能，因为外部已判断T_RETURN
    return nullptr;
}

/// 识别表达式尾部符号，文法： assignExprStmtTail : T_ASSIGN expr | ε
static ast_node * assignExprStmtTail(ast_node * left_node)
{
    if (match(T_ASSIGN)) {

        // 赋值运算符，说明含有赋值运算

        // 必须要求左侧节点必须存在
        if (!left_node) {

            // 没有左侧节点，则语法错误
            semerror("赋值语句的左侧表达式不能为空");

            return nullptr;
        }

        // 赋值运算符右侧表达式分析识别
        ast_node * right_node = expr();

        return create_contain_node(ast_operator_type::AST_OP_ASSIGN, left_node, right_node);
    } else if (F(T_SEMICOLON)) {
        // 看是否满足assignExprStmtTail的Follow集合。在Follow集合中则正常结束，否则出错
        // 空语句
    }

    return left_node;
}

///
/// @brief 赋值语句或表达式语句识别，文法：assignExprStmt : expr assignExprStmtTail
/// @return ast_node*
///
static ast_node * assignExprStmt()
{
    // 识别表达式，目前还不知道是否是表达式语句或赋值语句
    ast_node * expr_node = expr();

    return assignExprStmtTail(expr_node);
}

///
/// @brief 语句的识别，其文法为：
/// statement:T_RETURN expr T_SEMICOLON
/// @return AST的节点
///
static ast_node * statement()
{

    if (match(T_RETURN)) {

        // 匹配成功return关键字并移动到下一个字符

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
/// @brief 变量定义列表语法识别 其文法：varDeclList : T_COMMA T_ID varDeclList | T_SEMICOLON
/// @param vardeclstmt_node 变量声明语句节点，所有的变量节点应该加到该节点中
///
static void varDeclList(ast_node * vardeclstmt_node)
{
    if (match(T_COMMA)) {

        // 匹配成功，定义列表中有逗号

        // 检查是否是标识符
        if (F(T_ID)) {

            // 定义列表中定义的变量

            // 新建变量声明节点并加入变量声明语句中
            (void) add_var_decl_node(vardeclstmt_node, rd_lval.var_id);

            // 填过当前的Token，指向下一个Token
            advance();

            // 递归调用，不断追加变量定义
            varDeclList(vardeclstmt_node);
        } else {
            semerror("逗号后必须是标识符");
        }
    } else if (match(T_SEMICOLON)) {
        // 匹配成功，则说明只有前面的一个变量或者变量定义，正常结束
    } else {
        semerror("非法记号: %d", (int) lookaheadTag);

        // 忽略该记号，继续检查
        advance();

        // 继续检查后续的变量
        varDeclList(vardeclstmt_node);
    }
}

///
/// @brief 局部变量的识别，其文法为：
/// varDecl : T_INT T_ID varDeclList
///
/// @return ast_node* 局部变量声明节点
///
static ast_node * localVarDecl()
{
    if (F(T_INT)) {

        // 这里必须复制，而不能引用，因为rd_lval为全局，下一个记号识别后要被覆盖
        type_attr type = rd_lval.type;

        // 跳过int类型的记号，指向下一个Token
        advance();

        // 检测是否是标识符
        if (F(T_ID)) {

            // 创建变量声明语句，并加入第一个变量
            ast_node * stmt_node = create_var_decl_stmt_node(type, rd_lval.var_id);

            // 跳过标识符记号，指向下一个Token
            advance();

            varDeclList(stmt_node);

            return stmt_node;

        } else {
            semerror("类型后要求的记号为标识符");
            // 这里忽略继续检查下一个记号，为便于一次可检查出多个错误
            // 当然可以直接退出循环，一旦有错就不再检查语法错误。
        }
    }

    return nullptr;
}

///
/// @brief 块中的项目识别，其文法为：
/// blockItem: statement
static ast_node * BlockItem()
{
    return statement();
}

///
/// @brief 块语句列表分析识别，文法为BlockItemList : BlockItem+
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
/// @brief 语句块识别，文法：T_L_BRACE BlockItemList? T_R_BRACE
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

        // 遍历是否有语句定义？如有则追加
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
/// @brief 函数定义的识别
/// 函数定义的文法： funcDef: T_INT T_ID T_L_PAREN T_R_PAREN block
/// @return ast_node 函数运算符节点
///
static ast_node * funcDef()
{
    if (F(T_INT)) {

        // 函数返回之后类型
        type_attr type = rd_lval.type;

        // 跳过当前的记号，指向下一个记号
        advance();

        // 检测是否是标识符
        if (F(T_ID)) {

            // 获取标识符的值和定位信息
            var_id_attr id = rd_lval.var_id;

            // 跳过当前的记号，指向下一个记号
            advance();

            // 函数定义的左右括号识别
            if (match(T_L_PAREN)) {

                // 目前函数定义不执行形参，因此必须是右小括号
                if (match(T_R_PAREN)) {

                    // 识别block
                    ast_node * blockNode = Block();

                    // 根据第一个变量声明创建变量声明语句节点并加入其中
                    // 形参结点没有，设置为空指针
                    ast_node * formalParamsNode = nullptr;

                    // 创建函数定义的节点，孩子有类型，函数名，语句块和形参(实际上无)
                    return create_func_def(type, id, blockNode, formalParamsNode);

                } else {
                    semerror("函数定义缺少右小括号");
                }

            } else {
                semerror("函数定义缺少左小括号");
            }
        }
    }

    return nullptr;
}

// 编译单元识别，也就是文法的开始符号
// 其文法（antlr4中定义的）：
// compileUnit: funcDef EOF
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
