#pragma once

#include <algorithm>

struct Edge
{
    unsigned first, second;

    bool operator==(const Edge& e) const
    {
        bool equal = true;
        equal &= (std::min(first, second) == std::min(e.first, e.second));
        equal &= (std::max(first, second) == std::max(e.first, e.second));
        return equal;
    }
};

template<typename T>
inline void hash_combine(std::size_t& seed, const T& val)
{
    std::hash<T> hasher;
    seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<>
    struct hash<Edge>
    {
        inline size_t operator()(const Edge& val) const
        {
            size_t seed = 0;
            hash_combine(seed, std::max(val.first, val.second));
            hash_combine(seed, std::min(val.first, val.second));
            return seed;
        }
    };
}
