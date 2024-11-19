%{
#include <cstdio>
#include <cstring>

// 词法分析头文件
#include "FlexLexer.h"

// bison生成的头文件
#include "BisonParser.h"

// 抽象语法树函数定义原型头文件
#include "AST.h"

#include "IntegerType.h"

// LR分析失败时所调用函数的原型声明
void yyerror(char * msg);

%}

// 联合体声明，用于后续终结符和非终结符号属性指定使用
%union {
    class ast_node * node;

    struct digit_int_attr integer_num;
    struct digit_real_attr float_num;
    struct var_id_attr var_id;
	struct type_attr type;
	int op_class;
};

// 文法的开始符号
%start  CompileUnit

// 指定文法的终结符号，<>可指定文法属性
// 对于单个字符的算符或者分隔符，在词法分析时可直返返回对应的ASCII码值，bison预留了255以内的值
// %token开始的符号称之为终结符，需要词法分析工具如flex识别后返回
// %type开始的符号称之为非终结符，需要通过文法产生式来定义
// %token或%type之后的<>括住的内容成为文法符号的属性，定义在前面的%union中的成员名字。
%token <integer_num> T_DIGIT
%token <var_id> T_ID
%token <type> T_INT

// 关键或保留字 一词一类 不需要赋予语义属性
%token T_RETURN

// 分隔符 一词一类 不需要赋予语义属性
%token T_SEMICOLON T_L_PAREN T_R_PAREN T_L_BRACE T_R_BRACE

// 运算符

// 非终结符
// %type指定文法的非终结符号，<>可指定文法属性
%type <node> CompileUnit
%type <node> FuncDef
%type <node> Block
%type <node> BlockItemList
%type <node> BlockItem
%type <node> Statement
%type <node> Expr

%%
// 完整的编译单元语法如下
// 编译单元可包含若干个函数与全局变量定义。要在语义分析时检查main函数存在
// compileUnit: (funcDef | varDecl)* EOF;
// bison不支持闭包运算，为便于追加修改成左递归方式
// compileUnit: funcDef | varDecl | compileUnit funcDef | compileUnit varDecl
// 目前 编译单元内部仅仅支持识别一个函数
CompileUnit : FuncDef {

		// 创建一个编译单元的节点AST_OP_COMPILE_UNIT
        $$ = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT, $1);
        ast_root = $$;
    }
    ;

// 函数定义，目前支持整数返回类型，不支持形参
FuncDef : T_INT T_ID T_L_PAREN T_R_PAREN Block  {

        // 创建整型类型节点的终结符节点
        ast_node * type_node = ast_node::New(IntegerType::getTypeInt());

        // 创建函数名的标识符终结符节点
        ast_node * name_node = ast_node::New(std::string($2.id), $2.lineno);

		// 对于字符型字面量的字符串空间需要释放，因词法用到了strdup进行了字符串复制
		free($2.id);

        // 形参结点没有，设置为空指针
        ast_node * formalParamsNode = nullptr;

		// 函数体节点即Block，即$5

		// 创建函数定义的节点，孩子有类型，函数名，语句块和形参(实际上无)
        $$ = create_func_def(type_node, name_node, $5, formalParamsNode);
    }
    ;

// 语句块的文法Block ： T_L_BRACE BlockItemList? T_R_BRACE
// 其中?代表可有可无，在bison中不支持，需要拆分成两个产生式
// Block ： T_L_BRACE T_R_BRACE | T_L_BRACE BlockItemList T_R_BRACE
Block : T_L_BRACE T_R_BRACE {
		// 语句块没有语句
		// 为了方便创建一个空的Block节点
        $$ = create_contain_node(ast_operator_type::AST_OP_BLOCK);
    }
    | T_L_BRACE BlockItemList T_R_BRACE {
        // 语句块含有语句
		// BlockItemList归约时内部创建Block节点，并把语句加入，这里不创建Block节点
        $$ = $2;
    }
    ;

// 语句块内语句列表的文法：BlockItemList : BlockItem+
// Bison不支持正闭包，因此需要修改成左递归形式
// 左递归形式的文法为：BlockItemList : BlockItem | BlockItemList BlockItem
BlockItemList : BlockItem {
        // 第一个左侧的孩子节点归约成Block节点，后续语句可持续作为孩子追加到Block节点中
        // 创建一个AST_OP_BLOCK类型的中间节点，孩子为Statement($1)
		$$ = create_contain_node(ast_operator_type::AST_OP_BLOCK, $1);
    }
	| BlockItemList BlockItem {
		// 把BlockItem归约的节点加入到BlockItemList的节点中
		$$ = $1->insert_son_node($2);
	}
    ;


// 完整语句块子项的文法：BlockItem : Statement | VarDecl
// 目前只支持语句,不支持变量定义
BlockItem : Statement  {
		// 语句节点传递给归约后的节点上，综合属性
        $$ = $1;
    }
    ;


// 完整语句文法：statement:T_RETURN expr T_SEMICOLON | lVal T_ASSIGN expr T_SEMICOLON
// | block | expr? T_SEMICOLON
// 支持返回语句、赋值语句、语句块、表达式语句
// 其中表达式语句可支持空语句，由于bison不支持?，修改成两条
// 当前语句文法：statement:T_RETURN expr T_SEMICOLON,即仅支持return语句
Statement : T_RETURN Expr T_SEMICOLON {
        // 返回语句

		// 创建返回节点AST_OP_RETURN，其孩子为Expr，即$2
        $$ = create_contain_node(ast_operator_type::AST_OP_RETURN, $2);
    }
	;
	

// 表达式文法 expr : T_DIGIT
// 表达式目前只支持数字识别
Expr :T_DIGIT{
		$$ = ast_node::New(digit_int_attr{$1.val, $1.lineno});
	}
    ;

//TODO
%%

// 语法识别错误要调用函数的定义
void yyerror(char * msg)
{
    printf("Line %d: %s\n", yylineno, msg);
}
