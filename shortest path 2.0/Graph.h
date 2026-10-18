#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <stdexcept>

typedef std::vector<std::vector<std::pair<int, int> > > Graph;


inline void addEdge(Graph& g, int u, int v, int w) {
    int n = static_cast<int>(g.size());

    if (u < 0 || v < 0 || u >= n || v >= n)
        throw std::out_of_range("addEdge: vertex index out of range");

    if (w < 0)
        throw std::invalid_argument("addEdge: negative edge weight");

    g[u].push_back(std::make_pair(v, w));
    g[v].push_back(std::make_pair(u, w));
}

#endif
