///
/// @file Set.h
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
#pragma once

#include <cstdint>
#include <set>
#include <string>

///
/// @brief 集合类——使用位图表达集合运算
///
class Set {
protected:
    std::set<uint32_t> bitmap;
    uint32_t count;

public:
    ///
    /// @brief Construct a new Set object
    ///
    Set();

    ///
    /// @brief 从0开始的前count元素设置有效或者无效
    /// @param count 前count个数
    /// @param val 设置的值，真为1，假为0
    ///
    void init(uint32_t count, bool val);

    ///
    /// @brief 从[from, to]全部设置
    /// @param from 开始编号
    /// @param to 结束编号
    /// @param val 设置的值，真为1，假为0
    ///
    void init(uint32_t from, uint32_t to, bool val);

    ///
    /// @brief 现有的全部集合元素清除
    ///
    void clear();

    ///
    /// @brief 变换成字符串显示
    /// @return std::string 字符串
    ///
    std::string toString();

    ///
    /// @brief 交集运算
    /// @param val 参与交集的集合
    /// @return Set 交集后的集合
    ///
    Set operator&(Set val);

    ///
    /// @brief 并集运算
    /// @param val 参与运算集合
    /// @return Set 运算结果集合
    ///
    Set operator|(Set val);

    ///
    /// @brief 差集运算
    /// @param val 参与运算集合
    /// @return Set 运算结果集合
    ///
    Set operator-(Set val);

    ///
    /// @brief 异或运算
    /// @param val 参与运算集合
    /// @return Set 运算结果集合
    ///
    Set operator^(Set val);

    ///
    /// @brief 补集运算
    /// @return Set 运算结果集合
    ///
    Set operator~();

    ///
    /// @brief 交集运算
    /// @param val 参与运算的集合
    /// @return Set 运算后的集合
    ///
    Set & operator&=(Set & val);

    ///
    /// @brief 并集运算
    /// @param val 参与运算的集合
    /// @return Set 运算后的集合
    ///
    Set & operator|=(Set & val);

    ///
    /// @brief 差集运算
    /// @param val 参与运算的集合
    /// @return Set 运算后的集合
    ///
    Set & operator-=(Set & val);

    ///
    /// @brief 异或运算
    /// @param val 参与运算的集合
    /// @return Set 运算后的集合
    ///
    Set & operator^=(Set & val);

    ///
    /// @brief 比较运算（等于）
    /// @param val 参与运算的集合
    /// @return bool 等于为真，否则为假
    ///
    bool operator==(Set & val);

    ///
    /// @brief 比较运算（不等于）
    /// @param val 参与运算的集合
    /// @return bool 不等为真，否则为假
    ///
    bool operator!=(Set & val); //比较运算

    ///
    /// @brief 获取指定位的值
    /// @param n 指定位
    /// @return true 有值
    /// @return false 无值
    ///
    bool get(uint32_t n);

    ///
    /// @brief 置位运算
    /// @param n 指定位
    ///
    void set(uint32_t n);

    ///
    /// @brief 复位运算
    /// @param n
    ///
    void reset(uint32_t n);

    ///
    /// @brief 返回最高位的1的索引。请注意集合不要为空
    /// @return uint32_t 索引号
    ///
    uint32_t max();

    ///
    /// @brief 返回最低位的1的索引。请注意集合不要为空
    /// @return uint32_t 索引号
    ///
    uint32_t min();

    ///
    /// @brief 判断集合是否空
    /// @return true 空
    /// @return false 不空
    ///
    bool empty();
};
