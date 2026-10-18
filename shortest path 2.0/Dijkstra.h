#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include <vector>

const int INF = 1000000000;

struct ShortestPathResult {
    std::vector<int> dist;    // INF if unreachable
    std::vector<int> parent;  // previous vertex on the path, -1 if none
};

ShortestPathResult dijkstra(
    const Graph& graph,
    int source
);

std::vector<int> buildPath(
    const ShortestPathResult& result,
    int target
);

#endif
