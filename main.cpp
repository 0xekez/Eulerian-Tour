#include "UndirectedGraph.hpp"
#include "Edge.hpp"
#include "eulerianTour.hpp"

#include <list>

int main()
{
    // A couple linked up boxes.
    // UndirectedGraph<10> G(
    //     std::vector<Edge>({
    //         // First box.
    //         {0, 1},
    //         {1, 2},
    //         {2, 3},
    //         {3, 0},
    //         // Top box.
    //         {2, 4},
    //         {4, 5},
    //         {5, 6},
    //         {6, 2},
    //         // Bottom box.
    //         {3, 7},
    //         {7, 8},
    //         {8, 9},
    //         {9, 3}
    //     })
    // );

    // http://mathworld.wolfram.com/EulerianCycle.html
    UndirectedGraph<6> G(
        std::vector<Edge>(
            {
                // Outer triangle.
                {0, 1},
                {1, 2},
                {2, 0},
                // Inner triangle.
                {4, 5},
                {5, 3},
                {3, 4},
                // Connections.
                {4, 0},
                {5, 0},
                {4, 1},
                {3, 1},
                {3, 2},
                {5, 2}
            }
        )
    );

    std::list<Edge> tour( eulerianTour(G, 0) );

    for ( const auto& edge : tour)
    {
        printf("{ %u, %u } \t", edge.first, edge.second);
    }
    printf("\n");
}