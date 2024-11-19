/**
 * @file Common.cpp
 * @author zenglj (zenglj@nwpu.edu.cn)
 * @brief 共同函数
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <cstdint>
#include <string>
#include <iostream>

#include "Common.h"

using namespace std;

/// @brief 整数变字符串
/// @param num 无符号数
/// @return 字符串
string int2str(uint64_t num)
{
    return std::to_string(num);
}

/// @brief 浮点数变字符串
/// @param num 浮点数
/// @return 字符串
string double2str(double num)
{
    return std::to_string(num);
}

/// @brief 检查字符是否是字母（大小写字母）
/// @param ch 字符
/// @return true：是，false：不是
bool isLetter(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

/// @brief 检查字符是否为数字(0-9)
/// @param ch 字符
/// @return true：字符是数字字符(0-9) false:不是
bool isDigital(char ch)
{
    return (ch >= '0' && ch <= '9');
}

/// @brief 检查字符是否为大小写字符或数字(0-9)
/// @param ch 字符
/// @return true：是，false：不是
bool isLetterDigital(char ch)
{
    return isLetter(ch) || isDigital(ch);
}

/// @brief 检查字符是否为大小写字符或数字(0-9)或下划线
/// @param ch 字符
/// @return true：是，false: 不是
bool isLetterDigitalUnderLine(char ch)
{
    return isLetterDigital(ch) || (ch == '_');
}

/// @brief 检查字符是否为大小写字符或下划线
/// @param ch 字符
/// @return true：是，false：不是
bool isLetterUnderLine(char ch)
{
    return isLetter(ch) || (ch == '_');
}

/// @brief 删除前后空格
/// @param str 要处理的字符串
/// @return 处理后的字符串
std::string trim(const std::string & str)
{
    std::string::size_type pos = str.find_first_not_of(' ');
    if (pos == std::string::npos) {
        return str;
    }

    std::string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != std::string::npos) {
        return str.substr(pos, pos2 - pos + 1);
    }

    return str.substr(pos);
}

void minic_log_common(int level, const char * content)
{
    if (level != LOG_ERROR) {
        std::cerr << content << std::endl;
    } else {
        std::cout << content << std::endl;
    }
}