///
/// @file Set.cpp
/// @brief 集合类
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-19
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-19 <td>1.0     <td>zenglj  <td>新建
/// </table>
///

#include <sstream>
#include <algorithm>
#include <iostream>

#include "Set.h"

/*
    构造函数
*/
Set::Set()
{
    count = 0;
}

// 从0开始的前count元素设置有效或者无效
void Set::init(uint32_t _count, bool val)
{
    // 如果全部都设置为0，则可以直接clear即可，不用erase
    if (this->count >= _count && 0 == val) {
        this->count = _count;
        this->bitmap.clear();
        return;
    }

    this->count = _count;

    for (uint32_t k = 0; k < _count; k++) {
        if (val) {
            this->bitmap.insert(k);
        } else {
            this->bitmap.erase(k);
        }
    }
}

// 从[from, to]全部设置
void Set::init(uint32_t from, uint32_t to, bool val)
{
    for (uint32_t k = from; k < to; k++) {
        if (val) {
            this->bitmap.insert(k);
        } else {
            this->bitmap.erase(k);
        }
    }
}

void Set::clear()
{
    this->bitmap.clear();
}

/*
    交集运算
*/
Set Set::operator&(Set val)
{
    Set ret;

    set_intersection(std::begin(this->bitmap),
                     std::end(this->bitmap),
                     std::begin(val.bitmap),
                     std::end(val.bitmap),
                     std::inserter(ret.bitmap, std::begin(ret.bitmap)));

    ret.count = std::max(this->count, val.count);

    return ret;
}

/*
    并集运算
*/
Set Set::operator|(Set val)
{
    Set ret;

    set_union(std::begin(this->bitmap),
              std::end(this->bitmap),
              std::begin(val.bitmap),
              std::end(val.bitmap),
              std::inserter(ret.bitmap, std::begin(ret.bitmap)));

    ret.count = std::max(this->count, val.count);

    return ret;
}

/*
    差集运算
*/
Set Set::operator-(Set val)
{
    Set ret;

    set_difference(std::begin(this->bitmap),
                   std::end(this->bitmap),
                   std::begin(val.bitmap),
                   std::end(val.bitmap),
                   std::inserter(ret.bitmap, std::begin(ret.bitmap)));

    ret.count = std::max(this->count, val.count);

    return ret;
}

//异或运算
Set Set::operator^(Set val)
{
    Set ret;

    set_symmetric_difference(std::begin(this->bitmap),
                             std::end(this->bitmap),
                             std::begin(val.bitmap),
                             std::end(val.bitmap),
                             std::inserter(ret.bitmap, std::begin(ret.bitmap)));

    ret.count = std::max(this->count, val.count);

    return ret;
}

// 补集运算
Set Set::operator~()
{
    Set ret;

    for (uint32_t k = 0; k < count; k++) {
        if (this->bitmap.find(k) != this->bitmap.end()) {
            ret.bitmap.erase(k);
        } else {
            ret.bitmap.insert(k);
        }
    }

    ret.count = count;

    return ret;
}

/*
    比较运算
*/
bool Set::operator==(Set & val)
{
    return this->bitmap == val.bitmap;
}

/*
    比较运算
*/
bool Set::operator!=(Set & val)
{
    return this->bitmap != val.bitmap;
}

///
/// @brief 获取指定位的值
/// @param n 指定位
/// @return true 有值
/// @return false 无值
///
bool Set::get(uint32_t n)
{
    return bitmap.find(n) != bitmap.end();
}

/*
    置位运算
*/
void Set::set(uint32_t i)
{
    bitmap.insert(i);
}

///
/// @brief 置位运算
/// @param n 指定位
///
void Set::reset(uint32_t n)
{
    bitmap.erase(n);
}

///
/// @brief 返回最高位的1的索引
/// @return uint32_t 索引号。空返回-1
///
uint32_t Set::max()
{
    return *std::max_element(this->bitmap.begin(), this->bitmap.end());
}

///
/// @brief 返回最低位的1的索引
/// @return uint32_t 索引号。没有元素则返回-1
///
uint32_t Set::min()
{
    return *std::min_element(this->bitmap.begin(), this->bitmap.end());
}

/*
    调试输出函数
*/
std::string Set::toString()
{
    std::stringstream striostream;

    for (auto & el: bitmap) {
        striostream << el << " ";
    }

    return striostream.str();
}

bool Set::empty()
{
    return bitmap.empty();
}