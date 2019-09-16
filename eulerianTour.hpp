#pragma once

#include "UndirectedGraph.hpp"
#include "Edge.hpp"

#include <list>
#include <stack>
#include <array>
#include <unordered_set>

static void joinPaths(std::list<Edge>& start, const std::list<Edge>& add)
{
    if ( ! start.size() )
        {
        start.insert(start.end(), add.begin(), add.end());
        return;
        }

    auto joinPos = start.begin();

    while ( joinPos++->second != add.front().first )
        assert(joinPos != start.end() && "joinPaths given invalid paths.");

    start.insert(joinPos, add.begin(), add.end());
}

template <size_t SIZE>
static bool validateTour(UndirectedGraph<SIZE>& G, const std::list<Edge>& tour)
{
    bool valid = true;

    // Path must start and end at the same node.
    valid &= tour.front().first == tour.back().second;
    if ( ! valid )
    {
        printf("Tour must terminate at the start to be valid.\n");
        return valid;
    }

    // Validate that the tour doesn't have any duplicates.
    valid &= std::unordered_set<Edge>(tour.begin(), tour.end()).size() == tour.size();
    if ( ! valid )
    {
        printf("A valid tour can't contain any duplicate edges.\n");
        return valid;
    }

    // Validate that the path contains every edge.
    std::unordered_set<Edge> edges;
    for ( unsigned start = 0; start < SIZE; ++start )
    {
        auto& startNode = G.getNode(start);
        for ( unsigned end = 0; end < SIZE; ++end )
        {
            if ( end == start ) continue;

            if ( startNode.at(end) )
            {
                edges.insert({start, end});
            }
        }
    }

    valid &= tour.size() == edges.size();
    if ( ! valid )
    {
        printf("A valid tour must visit every edge.\n");
        return valid;
    }

    return valid;
}

template <size_t SIZE>
std::list<Edge>
eulerianTour(UndirectedGraph<SIZE>& G, unsigned start)
{
    std::unordered_set<Edge> visitedEdges;
    std::unordered_set<Edge> toVisit;
    std::list<Edge> tour;

    unsigned current = start;

    while ( true )
    {
        // Find a path.
        auto p = findPath(G, current, visitedEdges);

        // Add the path to our tour.
        joinPaths(tour, p.second);

        // Remove the path from the unvisited set.
        std::for_each(p.second.begin(), p.second.end(), [&toVisit](const Edge& e)
        {
            toVisit.erase(e);
        });

        // Move the path into our visited set.
        typedef std::list<Edge>::iterator iter_list;
        visitedEdges.insert(std::move_iterator<iter_list>(p.second.begin()), 
        std::move_iterator<iter_list>(p.second.end()));

        // Add the unvisited items to our set.
        while ( p.first.size() )
        {
            if ( visitedEdges.find(p.first.top()) == visitedEdges.end() )
                {
                    toVisit.insert(std::move(p.first.top()));
                }
            p.first.pop();
        }

        if ( ! toVisit.size() )
            break;

        // Get the next starting point.
        current = toVisit.begin()->first;
    }

    validateTour(G, tour);

    return tour;
}

// Trims detours from a path.
static std::list<Edge> trimPath(std::list<Edge> in)
{
    std::list<Edge> trimmed;

    auto loc = in.rbegin();
    auto start = --in.rend();

    trimmed.push_front(*loc);

    while ( loc != start )
    {
        unsigned goal = loc->first;
        while ( (++loc)->second != goal ) ;
        trimmed.push_front(std::move(*loc));
    }

    return trimmed;
}

template <size_t SIZE>
static std::pair<std::stack<Edge>, std::list<Edge>>
findPath(UndirectedGraph<SIZE> G, unsigned start, 
    const std::unordered_set<Edge>& globalVisits)
{
    typedef std::array<bool, SIZE> Node;

    std::list<Edge> path;
    std::unordered_set<Edge> localVisits;
    std::stack<Edge> toVisit;

    unsigned current = start;

    do
    {
        Node& loc = G.getNode(current);

        for (unsigned edge = 0; edge < SIZE; ++edge)
        {
            // if its connected
            if ( loc.at(edge) )
                // if its not visited
                if ( localVisits.find( {current, edge} ) == localVisits.end() &&
                    globalVisits.find( {current, edge} ) == globalVisits.end() )
                    {
                        toVisit.push({current, edge});
                    }
        }

        assert(toVisit.size() &&
            "Path must terminate at the start to be valid.");

        Edge next = toVisit.top();
        path.push_back(next);

        toVisit.pop();
        current = next.second;
        localVisits.emplace(std::move(next));
    }
    while (current != start);

    return {std::move(toVisit), trimPath(std::move(path))};
}