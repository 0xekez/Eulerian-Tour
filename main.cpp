// clang++ main.cpp -o main -O3 -Wl,-stack_size -Wl,0xF000000
// ./main > bigData.tsv
// Plot it however you'd like.
// ^ All the arrays are made on the stack so you need a lot of memory.

#include "UndirectedGraph.hpp"
#include "Edge.hpp"
#include "eulerianTour.hpp"

#include "Benchmark.hpp"

#include <list>
#include <chrono>
#include <time.h>

#define STEP 10

#define DO_2(F, start) F<start>(); F<start + STEP>();
#define DO_4(F, start) DO_2(F, start) DO_2(F, start + 2 * STEP)
#define DO_8(F, start) DO_4(F, start) DO_4(F, start + 4 * STEP)
#define DO_16(F, start) DO_8(F, start) DO_8(F, start + 8 * STEP)
#define DO_32(F, start) DO_16(F, start) DO_16(F, start + 16 * STEP)
#define DO_64(F, start) DO_32(F, start) DO_32(F, start + 32 * STEP)
#define DO_128(F, start) DO_64(F, start) DO_64(F, start + 64 * STEP)
#define DO_256(F, start) DO_128(F, start) DO_128(F, start + 128 * STEP)

// Usage examples.
static void octahedron();
static void basic();

template <size_t SIZE>
static void timeTour()
{
    auto G = generateGraph<SIZE>();

    auto start = std::chrono::steady_clock::now();
    eulerianTour(G, 0);
    auto end = std::chrono::steady_clock::now();

    printf("%lu\t%lld\n", SIZE, std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

int main()
{
    srand(time(0));
    DO_256(timeTour, 100)
}

static void octahedron()
{
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

static void basic()
{
    // A couple linked up boxes.
    UndirectedGraph<10> G(
        std::vector<Edge>({
            // First box.
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            // Top box.
            {2, 4},
            {4, 5},
            {5, 6},
            {6, 2},
            // Bottom box.
            {3, 7},
            {7, 8},
            {8, 9},
            {9, 3}
        })
    );

    std::list<Edge> tour( eulerianTour(G, 0) );

    for ( const auto& edge : tour)
    {
        printf("{ %u, %u } \t", edge.first, edge.second);
    }
    printf("\n");
}