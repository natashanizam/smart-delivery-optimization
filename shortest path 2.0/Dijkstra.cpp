#include "Dijkstra.h"

#include <queue>
#include <functional>
#include <algorithm>

using namespace std;


ShortestPathResult dijkstra(
    const Graph& graph,
    int source
) {
    int V = static_cast<int>(graph.size());

    ShortestPathResult result;
    result.dist.assign(V, INF);
    result.parent.assign(V, -1);

    if (source < 0 || source >= V)
        return result;

    typedef pair<int, int> State;   // (distance, vertex)

    priority_queue<State, vector<State>, greater<State> > pq;

    result.dist[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {

        State top = pq.top();
        pq.pop();

        int d = top.first;
        int u = top.second;

        // Stale queue entry
        if (d > result.dist[u])
            continue;

        for (size_t i = 0; i < graph[u].size(); i++) {

            int v = graph[u][i].first;
            int w = graph[u][i].second;

            if (d + w < result.dist[v]) {

                result.dist[v] = d + w;
                result.parent[v] = u;

                pq.push(make_pair(result.dist[v], v));
            }
        }
    }

    return result;
}


vector<int> buildPath(
    const ShortestPathResult& result,
    int target
) {
    vector<int> path;

    int V = static_cast<int>(result.dist.size());

    if (target < 0 || target >= V || result.dist[target] == INF)
        return path;

    for (int v = target; v != -1; v = result.parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());

    return path;
}
