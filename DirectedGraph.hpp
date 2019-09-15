#pragma once

#include "Graph.hpp"

#include <array>
#include <utility>
#include <vector>
#include <memory>

template <size_t SIZE>
class DirectedGraph : public Graph<SIZE>
{
private:
    typedef std::array<bool, SIZE> Node;
    std::array<Node, SIZE> _nodes;

public:
    DirectedGraph() = default;
    DirectedGraph(std::vector<Edge> connections);

    void connect(unsigned node1, unsigned node2) override;
    bool connected(unsigned node1, unsigned node2) override;

    void disconnect(unsigned node1, unsigned node2) override;

    Node& getNode(unsigned node) override { return _nodes[node]; }
};

template <size_t SIZE>
DirectedGraph<SIZE>::DirectedGraph(std::vector<Edge> connections)
: DirectedGraph()
{
    for ( const auto& pair : connections )
    {
        connect(pair.first, pair.second);
    }
}

template <size_t SIZE>
void DirectedGraph<SIZE>::connect(unsigned node1, unsigned node2)
{
    _nodes.at(node1).at(node2) = true;
}

template <size_t SIZE>
bool DirectedGraph<SIZE>::connected(unsigned node1, unsigned node2)
{
    return _nodes.at(node1).at(node2);
}

template <size_t SIZE>
void DirectedGraph<SIZE>::disconnect(unsigned node1, unsigned node2)
{
    _nodes.at(node1).at(node2) = false;
}