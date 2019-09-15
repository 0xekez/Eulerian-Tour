#pragma once

#include <array>

template <size_t SIZE>
class Graph
{
typedef std::array<bool, SIZE> Node;

public:
    virtual void connect(unsigned node1, unsigned node2) = 0;
    virtual bool connected(unsigned node1, unsigned node2) = 0;

    virtual void disconnect(unsigned node1, unsigned node2) = 0;

    virtual Node& getNode(unsigned node) = 0;
};