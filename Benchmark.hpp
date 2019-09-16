// Functions for benchmarking this implementation.
// Generates a valid tour of length N then adds some random connections to
// the graph.

#pragma once

#include "Edge.hpp"
#include "UndirectedGraph.hpp"

#include <array>
#include <random>
#include <functional>
#include <unordered_set>
#include <algorithm>
#include <chrono>

template<class Container>
static auto selectRandom(Container& l)
{
    unsigned where = rand() % l.size();
    auto it = std::begin(l);
    std::advance(it, where);
    return *it;
}

template<class T>
static T popRandom(std::list<T>& l)
{
    unsigned where = rand() % l.size();
    auto it = l.begin();
    std::advance(it, where);
    T item = *it;
    l.erase(it);
    return item;
}

template<class T>
static T popTarget(std::list<T>& l, T target)
{
    auto where = std::find(l.begin(), l.end(), target);
    assert( where != l.end() && "Invalid pop.");
    T item = *where;
    l.erase(where);
    return item;
}

// Generates a graph with a valid eulerian tour.
//     1. Select two nodes.
//     2. Connect them.
//
//     3. Selet a connected node.
//     4. Connect it to an unconnected node.
//     5. Repeat 3 and 4 until all nodes are connected.
//
//     6. Classify each node as even or odd.
//     7. Select an odd node.
//     8. Connect it with another valid odd node.
//     9. If there are no other valid odd nodes:
//          - Connect it to a random even one.
//     10. Repeat 7 - 9 until there are no more odd nodes.
template<size_t SIZE>
UndirectedGraph<SIZE> generateGraph()
{
    UndirectedGraph<SIZE> G;

    std::list<unsigned> connectedNodes;
    std::list<unsigned> disconnectedNodes(SIZE);
    // Put all the nodes in our disconnected list.
    std::iota(disconnectedNodes.begin(), disconnectedNodes.end(), 0);

    // 1. Select two nodes.
    unsigned node1 = popRandom(disconnectedNodes);
    unsigned node2 = popRandom(disconnectedNodes);

    // 2. Connect them.
    G.connect(node1, node2);

    connectedNodes.push_back(node1);
    connectedNodes.push_back(node2);

    // 5. Repeat 3 and 4 until all nodes are connected.
    while ( disconnectedNodes.size() )
    {
        // 3. Selet a connected node.
        unsigned node1 = selectRandom(connectedNodes);

        // 4. Connect it to an unconnected node.
        unsigned node2 = popRandom(disconnectedNodes);
        G.connect(node1, node2);
        connectedNodes.push_back(node2);
    }

    // 6. Classify each node as even or odd.
    auto isOddNode = [](decltype(G)& G, int node) -> bool
    {
        typedef std::array<bool, SIZE> Node;
        Node& n = G.getNode(node);
        int n_connections = std::count(n.begin(), n.end(), true);
        return n_connections & 1;
    };
    auto odd = std::bind(isOddNode, G, std::placeholders::_1);

    std::list<unsigned> oddNodes;
    std::list<unsigned> evenNodes;

    for (unsigned n : connectedNodes)
    {
        if ( odd(n) )
            oddNodes.push_back(n);
        else evenNodes.push_back(n);
    }

    // 10. Repeat 7 - 9 until there are no more odd nodes.
    while ( oddNodes.size() )
    {
        // 7. Select an odd node.
        unsigned on = popRandom(oddNodes);
        auto Node = G.getNode(on);

        // 8. Connect it with another valid odd node.
        bool foundNode = false;
        for ( unsigned node : oddNodes )
        {
            if ( node == on ) continue;

            // If the nodes aren't connected, connect them.
            if ( ! Node.at(node) )
            {
                G.connect(on, node);
                evenNodes.push_back(on);
                evenNodes.push_back(popTarget(oddNodes, node));
                foundNode = true;
                break;
            }
        }

        // 9. If there are no other valid odd nodes:
        //        - Connect it to a random even one.
        if ( ! foundNode )
        {
            for ( unsigned node : evenNodes )
            {
                if ( ! Node.at(node) )
                {
                    G.connect(on, node);
                    // Make the even node odd.
                    popTarget(evenNodes, node);
                    oddNodes.push_back(node);
                    // Make the odd node even.
                    evenNodes.push_back(on);
                    foundNode = true;
                    break;
                }
            }
        }

        // Sometimes this won't work. But there exists a way that it does
        // work... ;-)
        if ( ! foundNode ) return generateGraph<SIZE>();
    }
    return G;
}

template <size_t SIZE>
void timeTour()
{
    auto G = generateGraph<SIZE>();

    auto start = std::chrono::steady_clock::now();
    eulerianTour(G, 0);
    auto end = std::chrono::steady_clock::now();

    printf("%lu\t%lld\n", SIZE, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}