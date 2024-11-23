grammar MiniC;

// 词法规则名总是以大写字母开头

// 语法规则名总是以小写字母开头

// 每个非终结符尽量多包含闭包、正闭包或可选符等的EBNF范式描述

// 若非终结符由多个产生式组成，则建议在每个产生式的尾部追加# 名称来区分，详细可查看非终结符statement的描述

// 语法规则描述：EBNF范式

// 源文件编译单元定义，目前只支持一个函数定义。 如需要支持多个，请修改语法产生式
compileUnit: funcDef;

// 函数定义，目前不支持形参，也不支持返回void类型等
funcDef: T_INT T_ID T_L_PAREN T_R_PAREN block;

// 语句块看用作函数体，这里允许多个语句，并且不含任何语句
block: T_L_BRACE blockItemList? T_R_BRACE;

// 每个ItemList可包含至少一个Item
blockItemList: blockItem+;

// 每个Item可以是一个语句 TODO 变量声明也是
blockItem: statement;

//TODO 变量声明

// 目前语句支持return和赋值语句
statement: T_RETURN expr T_SEMICOLON         # returnStatement;

// 目前表达式只支持一个无符号整数
expr: T_DIGIT;

// 用正规式来进行词法规则的描述

T_L_PAREN: '(';
T_R_PAREN: ')';
T_SEMICOLON: ';';
T_L_BRACE: '{';
T_R_BRACE: '}';

// 要注意关键字同样也属于T_ID，因此必须放在T_ID的前面，否则会识别成T_ID
T_RETURN: 'return';
T_INT: 'int';

T_ID: [a-zA-Z_][a-zA-Z0-9_]*;
T_DIGIT: '0' | [1-9][0-9]*;

/* 空白符丢弃 */
WS: [ \r\n\t]+ -> skip;