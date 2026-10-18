#ifndef MST_H
#define MST_H

#include "Graph.h"
#include <vector>
#include <string>

struct MSTEdge {
    int from;
    int to;
    int weight;
};

struct MSTResult {
    int totalCost;
    std::vector<MSTEdge> edges;
    bool connected;  
};


MSTResult primMST(
    const Graph& graph,
    int start
);

void printMST(
    const MSTResult& result,
    const std::vector<std::string>& cities
);

#endif
