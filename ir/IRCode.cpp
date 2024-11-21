///
/// @file IRCode.cpp
/// @brief IR指令序列类实现
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
#include "IRCode.h"

/// @brief 析构函数
InterCode::~InterCode()
{
    Delete();
}

/// @brief 添加一个指令块，添加到尾部，并清除原来指令块的内容
/// @param block 指令块，请注意加入后会自动清空block的指令
void InterCode::addInst(InterCode & block)
{
    std::vector<Instruction *> & insert = block.getInsts();

    code.insert(code.end(), insert.begin(), insert.end());

    // InterCode析构会清理资源，因此移动指令到code中后必须清理，否则会释放多次导致程序例外
    // 当然，这里也可不清理，但InterCode的析构函数不能清理，需专门的函数清理即可。
    insert.clear();
}

/// @brief 添加一条中间指令
/// @param inst IR指令
void InterCode::addInst(Instruction * inst)
{
    code.push_back(inst);
}

/// @brief 获取指令序列
/// @return 指令序列
std::vector<Instruction *> & InterCode::getInsts()
{
    return code;
}

/// @brief 删除所有指令
void InterCode::Delete()
{
    // 不能直接删除指令，需要先清除操作数
    for (auto inst: code) {
        inst->clearOperands();
    }

    // 资源清理
    for (auto inst: code) {
        delete inst;
    }

    code.clear();
}
