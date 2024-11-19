///
/// @file BitMap.h
/// @brief 位图类
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
#include <cstdint>
#include <vector>

///
/// @brief 非类型模板参数，容量N个字节
/// @tparam N 容量N个字节
///
template <std::size_t N>
class BitMap {
public:
    ///
    /// @brief Construct a new BitMap object
    ///
    BitMap()
    {
        // 开辟最少一个字节的空间
        _bits.resize(N / 8 + 1, 0);
    }

    ///
    /// @brief 置位函数，将指定的位设置为1
    /// @param x
    ///
    void set(std::size_t x)
    {
        // x/8 表明i是在vector中的哪一个字节当中
        std::size_t i = x / 8;

        // j表明 在 vector 第i个字节中的第几位
        std::size_t j = x % 8;

        // 把vector 第i个字节中的第j位置1；
        _bits[i] |= 1 << j;
    }

    //复位函数，将指定位设置为0
    void reset(std::size_t x)
    {
        // x/8 表明i是在vector中的哪一个字节当中
        std::size_t i = x / 8;

        // j表明 在 vector 第i个字节中的第几位
        std::size_t j = x % 8;

        // 把vector 第i个字节中的第j位置0；
        _bits[i] &= ~(1 << j);
    }

    ///
    /// @brief 访问函数，获取指定的位的值
    /// @param x 指定的位数
    /// @return true 在
    /// @return false 不在
    ///
    bool test(std::size_t x)
    {
        // x/8 表明i是在vector中的哪一个字节当中
        std::size_t i = x / 8;

        // j表明 在 vector 第i个字节中的第几位
        std::size_t j = x % 8;

        // 用& 只查看 不修改
        return _bits[i] & (1 << j);
    }

private:
    ///
    /// @brief 保存位图的数据
    ///
    std::vector<char> _bits;
};
