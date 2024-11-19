/**
 * @file AST.h
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 抽象语法树AST管理的源文件
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "AttrType.h"
#include "IRCode.h"
#include "Value.h"
#include "VoidType.h"

///
/// @brief AST节点的类型。C++专门因为枚举类来区分C语言的结构体
///
enum class ast_operator_type : int {

    /* 以下为AST的叶子节点 */

    /// @brief 无符号整数字面量叶子节点
    AST_OP_LEAF_LITERAL_UINT,

    /// @brief  无符号整数字面量叶子节点
    AST_OP_LEAF_LITERAL_FLOAT,

    /// @brief 变量ID叶子节点
    AST_OP_LEAF_VAR_ID,

    /// @brief 复杂类型的节点
    AST_OP_LEAF_TYPE,

    /* 以下为AST的内部节点，含根节点 */
    /// @brief 二元运算符+
    AST_OP_ADD,

    /// @brief 二元运算符*
    AST_OP_SUB, //

    /// @brief 多个语句组成的块运算符，也称为复合语句
    AST_OP_BLOCK,

    /// @brief 符合语句，也就是语句块，两个名字一个运算符
    AST_OP_COMPOUNDSTMT = AST_OP_BLOCK,

    /// @brief 赋值语句运算符
    AST_OP_ASSIGN,

    /// @brief return语句运算符
    AST_OP_RETURN,

    /// @brief 函数定义运算符，函数名和返回值类型作为节点的属性，自左到右孩子：AST_OP_FUNC_FORMAL_PARAMS、AST_OP_BLOCK
    AST_OP_FUNC_DEF,

    /// @brief 形式参数列表运算符，可包含多个孩子：AST_OP_FUNC_FORMAL_PARAM
    AST_OP_FUNC_FORMAL_PARAMS,

    /// @brief 形参运算符，属性包含名字与类型，复杂类型时可能要包含孩子
    AST_OP_FUNC_FORMAL_PARAM,

    /// @brief 函数调用运算符，函数名作为节点属性，孩子包含AST_OP_FUNC_REAL_PARAMS
    AST_OP_FUNC_CALL,

    /// @brief 实际参数列表运算符，可包含多个表达式AST_OP_EXPR
    AST_OP_FUNC_REAL_PARAMS,

    /// @brief 文件编译单元运算符，可包含函数定义、语句块等孩子
    AST_OP_COMPILE_UNIT,

    /// @brief 变量声明语句
    AST_OP_DECL_STMT,

    /// @brief 变量声明
    AST_OP_VAR_DECL,

    // TODO 抽象语法树其它内部节点运算符追加

    /// @brief 最大标识符，表示非法运算符
    AST_OP_MAX,
};

///
/// @brief 抽象语法树AST的节点描述类
///
class ast_node {
public:
    /// @brief 节点类型
    ast_operator_type node_type;

    /// @brief 行号信息，主要针对叶子节点有用
    int64_t line_no;

    /// @brief 节点值的类型，可用于函数返回值类型
    Type * type;

    /// @brief 无符号整数字面量值
    uint32_t integer_val;

    /// @brief float类型字面量值
    float float_val;

    /// @brief 变量名，或者函数名
    std::string name;

    /// @brief 父节点
    ast_node * parent = nullptr;

    /// @brief 孩子节点
    std::vector<ast_node *> sons;

    /// @brief 线性IR指令块，可包含多条IR指令，用于线性IR指令产生用
    InterCode blockInsts;

    /// @brief 线性IR指令或者运行产生的Value，用于线性IR指令产生用
    Value * val = nullptr;

    ///
    /// @brief 在进入block等节点时是否要进行作用域管理。默认要做。
    ///
    bool needScope = true;

    /// @brief 创建指定节点类型的节点
    /// @param _node_type 节点类型
    ast_node(ast_operator_type _node_type, Type * _type = VoidType::getType(), int64_t _line_no = -1);

    /// @brief 构造函数
    /// @param _type 节点值的类型
    ast_node(Type * _type);

    /// @brief 针对无符号整数字面量的构造函数
    /// @param attr 无符号整数字面量
    ast_node(digit_int_attr attr);

    /// @brief 针对标识符ID的叶子构造函数
    /// @param attr 字符型标识符
    ast_node(var_id_attr attr);

    /// @brief 针对标识符ID的叶子构造函数
    /// @param _id 标识符ID
    /// @param _line_no 行号
    ast_node(std::string id, int64_t _line_no);

    /// @brief 判断是否是叶子节点
    /// @param type 节点类型
    /// @return true：是叶子节点 false：内部节点
    bool isLeafNode();

    /// @brief 向父节点插入一个节点
    /// @param parent 父节点
    /// @param node 节点
    ast_node * insert_son_node(ast_node * node);

    /// @brief 创建指定节点类型的节点，最后一个孩子节点必须指定为nullptr。
    /// @param type 节点类型
    /// @param  可变参数，最后一个孩子节点必须指定为nullptr。如果没有孩子，则指定为nullptr
    /// @return 创建的节点
    static ast_node * New(ast_operator_type type, ...);

    /// @brief 创建无符号整数的叶子节点
    /// @param val 词法值
    /// @param line_no 行号
    static ast_node * New(digit_int_attr attr);

    /// @brief 创建标识符的叶子节点
    /// @param val 词法值
    /// @param line_no 行号
    static ast_node * New(var_id_attr attr);

    /// @brief 创建标识符的叶子节点
    /// @param id 词法值
    /// @param line_no 行号
    static ast_node * New(std::string id, int64_t lineno);

    /// @brief 创建具备指定类型的节点
    /// @param type 节点值类型
    /// @param line_no 行号
    /// @return 创建的节点
    static ast_node * New(Type * type);

    ///
    /// @brief 释放节点
    /// @param node
    ///
    static void Delete(ast_node * node);
};

/// @brief AST资源清理
void free_ast(ast_node * root);

/// @brief抽象语法树的根节点指针
extern ast_node * ast_root;

/// @brief 创建AST的内部节点，请注意可追加孩子节点，请按次序依次加入，最多3个
/// @param node_type 节点类型
/// @param first_child 第一个孩子节点
/// @param second_child 第一个孩子节点
/// @param third_child 第一个孩子节点
/// @return 创建的节点
ast_node * create_contain_node(ast_operator_type node_type,
                               ast_node * first_child = nullptr,
                               ast_node * second_child = nullptr,
                               ast_node * third_child = nullptr);

/// @brief 创建函数定义类型的内部AST节点
/// @param type_node 函数返回值类型
/// @param name_node 函数名节点
/// @param block 函数体语句块
/// @param params 函数形参，可以没有参数
/// @return 创建的节点
ast_node *
create_func_def(ast_node * type_node, ast_node * name_node, ast_node * block = nullptr, ast_node * params = nullptr);

/// @brief 创建函数定义类型的内部AST节点
/// @param type 返回值类型
/// @param id 函数名字
/// @param block_node 函数体语句块节点
/// @param params_node 函数形参，可以没有参数
/// @return 创建的节点
ast_node * create_func_def(type_attr & type, var_id_attr & id, ast_node * block_node, ast_node * params_node);

/// @brief 创建函数形式参数的节点
/// @param line_no 行号
/// @param param_name 形式参数名
/// @return 创建的节点
ast_node * create_func_formal_param(uint32_t line_no, const char * param_name);

/// @brief 创建函数调用的节点
/// @param funcname_node 函数名节点
/// @param params_node 实参节点
/// @return 创建的节点
ast_node * create_func_call(ast_node * funcname_node, ast_node * params_node = nullptr);

/// @brief 创建类型节点
/// @param type 类型信息
/// @return 创建的节点
ast_node * create_type_node(type_attr & type);

///
/// @brief 类型属性转换成Type
/// @param attr 词法属性
/// @return Type* 类型
///
Type * typeAttr2Type(type_attr & attr);

///
/// @brief 根据第一个变量定义创建变量声明语句节点
/// @param first_child 第一个变量定义节点
/// @return ast_node* 变量声明语句节点
///
ast_node * create_var_decl_stmt_node(ast_node * first_child);

///
/// @brief 根据变量的类型和属性创建变量声明语句节点
/// @param type 变量的类型
/// @param id 变量的名字
/// @return ast_node* 变量声明语句节点
///
ast_node * create_var_decl_stmt_node(type_attr & type, var_id_attr & id);

///
/// @brief 向变量声明语句中追加变量声明
/// @param stmt_node 变量声明语句
/// @param id 变量的名字
/// @return ast_node* 变量声明语句节点
///
ast_node * add_var_decl_node(ast_node * stmt_node, var_id_attr & id);