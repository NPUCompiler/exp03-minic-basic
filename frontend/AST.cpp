///
/// @file AST.cpp
/// @brief 抽象语法树AST管理的实现
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
#include <cstdarg>
#include <cstdint>
#include <string>

#include "AST.h"
#include "AttrType.h"
#include "Types/IntegerType.h"
#include "Types/VoidType.h"

/* 整个AST的根节点 */
ast_node * ast_root = nullptr;

/// @brief 创建指定节点类型的节点
/// @param _node_type 节点类型
/// @param _line_no 行号
ast_node::ast_node(ast_operator_type _node_type, Type * _type, int64_t _line_no)
    : node_type(_node_type), line_no(-1), type(_type)
{}

/// @brief 构造函数
/// @param _type 节点值的类型
/// @param line_no 行号
ast_node::ast_node(Type * _type) : ast_node(ast_operator_type::AST_OP_LEAF_TYPE, _type, -1)
{}

/// @brief 针对无符号整数字面量的构造函数
/// @param attr 无符号整数字面量
ast_node::ast_node(digit_int_attr attr)
    : ast_node(ast_operator_type::AST_OP_LEAF_LITERAL_UINT, IntegerType::getTypeInt(), attr.lineno)
{
    integer_val = attr.val;
}

/// @brief 针对标识符ID的叶子构造函数
/// @param attr 字符型字面量
ast_node::ast_node(var_id_attr attr) : ast_node(ast_operator_type::AST_OP_LEAF_VAR_ID, VoidType::getType(), attr.lineno)
{
    name = attr.id;
}

/// @brief 针对标识符ID的叶子构造函数
/// @param _id 标识符ID
/// @param _line_no 行号
ast_node::ast_node(std::string _id, int64_t _line_no)
    : ast_node(ast_operator_type::AST_OP_LEAF_VAR_ID, VoidType::getType(), _line_no)
{
    name = _id;
}

/// @brief 判断是否是叶子节点
/// @return true：是叶子节点 false：内部节点
bool ast_node::isLeafNode()
{
    bool is_leaf;

    switch (this->node_type) {
        case ast_operator_type::AST_OP_LEAF_LITERAL_UINT:
        case ast_operator_type::AST_OP_LEAF_LITERAL_FLOAT:
        case ast_operator_type::AST_OP_LEAF_VAR_ID:
        case ast_operator_type::AST_OP_LEAF_TYPE:
            is_leaf = true;
            break;
        default:
            is_leaf = false;
            break;
    }

    return is_leaf;
}

/// @brief 创建指定节点类型的节点，请注意在指定有效的孩子后必须追加一个空指针nullptr，表明可变参数结束
/// @param type 节点类型
/// @param son_num 孩子节点的个数
/// @param ...
/// 可变参数，可支持插入若干个孩子节点，自左往右的次序，最后一个孩子节点必须指定为nullptr。如果没有孩子，则指定为nullptr
/// @return 创建的节点
ast_node * ast_node::New(ast_operator_type type, ...)
{
    ast_node * parent_node = new ast_node(type);

    va_list valist;

    /* valist指向传入的第一个可选参数 */
    va_start(valist, type);

    for (;;) {

        // 获取节点对象。如果最后一个对象为空指针，则说明结束
        ast_node * node = va_arg(valist, ast_node *);
        if (nullptr == node) {
            break;
        }

        // 插入到父节点中
        parent_node->insert_son_node(node);
    }

    /* 清理为 valist 保留的内存 */
    va_end(valist);

    return parent_node;
}

/// @brief 向父节点插入一个节点
/// @param parent 父节点
/// @param node 节点
ast_node * ast_node::insert_son_node(ast_node * node)
{
    if (node) {

        // 孩子节点有效时加入，主要为了避免空语句等时会返回空指针
        node->parent = this;
        this->sons.push_back(node);
    }

    return this;
}

/// @brief 创建无符号整数的叶子节点
/// @param attr 无符号整数字面量
ast_node * ast_node::New(digit_int_attr attr)
{
    ast_node * node = new ast_node(attr);

    return node;
}

/// @brief 创建标识符的叶子节点
/// @param attr 字符型字面量
ast_node * ast_node::New(var_id_attr attr)
{
    ast_node * node = new ast_node(attr);

    return node;
}

/// @brief 创建标识符的叶子节点
/// @param id 词法值
/// @param line_no 行号
ast_node * ast_node::New(std::string id, int64_t lineno)
{
    ast_node * node = new ast_node(id, lineno);

    return node;
}

/// @brief 创建具备指定类型的节点
/// @param type 节点值类型
/// @param line_no 行号
/// @return 创建的节点
ast_node * ast_node::New(Type * type)
{
    ast_node * node = new ast_node(type);

    return node;
}

/// @brief 递归清理抽象语法树
/// @param node AST的节点
void ast_node::Delete(ast_node * node)
{
    if (node) {

        for (auto child: node->sons) {
            ast_node::Delete(child);
        }

        // 这里没有必要清理孩子，由于下面就要删除该节点
        // node->sons.clear();
    }

    // 清理node资源
    delete node;
}

///
/// @brief AST资源清理
///
void free_ast(ast_node * root)
{
    ast_node::Delete(root);
}

/// @brief 创建函数定义类型的内部AST节点
/// @param type_node 类型节点
/// @param name_node 函数名字节点
/// @param block_node 函数体语句块节点
/// @param params_node 函数形参，可以没有参数
/// @return 创建的节点
ast_node * create_func_def(ast_node * type_node, ast_node * name_node, ast_node * block_node, ast_node * params_node)
{
    ast_node * node = new ast_node(ast_operator_type::AST_OP_FUNC_DEF, type_node->type, name_node->line_no);

    // 设置函数名
    node->name = name_node->name;

    // 如果没有参数，则创建参数节点
    if (!params_node) {
        params_node = new ast_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS);
    }

    // 如果没有函数体，则创建函数体，也就是语句块
    if (!block_node) {
        block_node = new ast_node(ast_operator_type::AST_OP_BLOCK);
    }

    (void) node->insert_son_node(type_node);
    (void) node->insert_son_node(name_node);
    (void) node->insert_son_node(params_node);
    (void) node->insert_son_node(block_node);

    return node;
}

/// @brief 创建函数定义类型的内部AST节点
/// @param type 返回值类型
/// @param id 函数名字
/// @param block_node 函数体语句块节点
/// @param params_node 函数形参，可以没有参数
/// @return 创建的节点
ast_node * create_func_def(type_attr & type, var_id_attr & id, ast_node * block_node, ast_node * params_node)
{
    // 创建整型类型节点的终结符节点
    ast_node * type_node = create_type_node(type);

    // 创建标识符终结符节点
    ast_node * id_node = ast_node::New(id.id, id.lineno);

    // 对于字符型字面量的字符串空间需要释放，因词法用到了strdup进行了字符串复制
    free(id.id);
    id.id = nullptr;

    return create_func_def(type_node, id_node, block_node, params_node);
}

/// @brief 创建AST的内部节点
/// @param node_type 节点类型
/// @param first_child 第一个孩子节点
/// @param second_child 第一个孩子节点
/// @param third_child 第一个孩子节点
/// @return 创建的节点
ast_node * create_contain_node(ast_operator_type node_type,
                               ast_node * first_child,
                               ast_node * second_child,
                               ast_node * third_child)
{
    ast_node * node = new ast_node(node_type);

    if (first_child) {
        (void) node->insert_son_node(first_child);
    }

    if (second_child) {
        (void) node->insert_son_node(second_child);
    }

    if (third_child) {
        (void) node->insert_son_node(third_child);
    }

    return node;
}

Type * typeAttr2Type(type_attr & attr)
{
    if (attr.type == BasicType::TYPE_INT) {
        return IntegerType::getTypeInt();
    } else {
        return VoidType::getType();
    }
}

/// @brief 创建类型节点
/// @param type 类型信息
/// @return 创建的节点
ast_node * create_type_node(type_attr & attr)
{
    Type * type = typeAttr2Type(attr);

    ast_node * type_node = ast_node::New(type);

    return type_node;
}
