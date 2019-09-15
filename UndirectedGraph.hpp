#pragma once

#include "Graph.hpp"
#include "Edge.hpp"

#include <array>
#include <utility>
#include <vector>
#include <memory>

template <size_t SIZE>
class UndirectedGraph : public Graph<SIZE>
{
private:
    typedef std::array<bool, SIZE> Node;
    std::array<Node, SIZE> _nodes;

public:
    UndirectedGraph() = default;
    UndirectedGraph(std::vector<Edge> connections);

    void connect(unsigned node1, unsigned node2) override;
    bool connected(unsigned node1, unsigned node2) override;

    void disconnect(unsigned node1, unsigned node2) override;

    Node& getNode(unsigned node) override { return _nodes[node]; }
};

template <size_t SIZE>
UndirectedGraph<SIZE>::UndirectedGraph(std::vector<Edge> connections)
: UndirectedGraph()
{
    for ( const auto& pair : connections )
    {
        connect(pair.first, pair.second);
    }
}

template <size_t SIZE>
void UndirectedGraph<SIZE>::connect(unsigned node1, unsigned node2)
{
    _nodes.at(node1).at(node2) = true;
    _nodes.at(node2).at(node1) = true;
}

template <size_t SIZE>
bool UndirectedGraph<SIZE>::connected(unsigned node1, unsigned node2)
{
    return _nodes.at(node1).at(node2);
}

template <size_t SIZE>
void UndirectedGraph<SIZE>::disconnect(unsigned node1, unsigned node2)
{
    _nodes.at(node1).at(node2) = false;
    _nodes.at(node2).at(node1) = false;
}