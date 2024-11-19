///
/// @file StorageSet.h
/// @brief 存储集合类
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///
#pragma once

#include <unordered_set>

template <typename T, typename Hasher, typename Equal>
class StorageSet final {
    std::unordered_set<T, Hasher, Equal> mStorage;

public:
    template <typename... Args>
    const T * get(Args &&... args)
    {
        return &*mStorage.emplace(std::forward<Args>(args)...).first;
    }
};
