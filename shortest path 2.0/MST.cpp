#include "MST.h"

#include <iostream>
#include <queue>
#include <tuple>
#include <functional>
#include <vector>
#include <string>

using namespace std;


MSTResult primMST(
    const Graph& graph,
    int start
) {
    int V = static_cast<int>(graph.size());

    MSTResult result;
    result.totalCost = 0;
    result.connected = (V == 0);

    if (start < 0 || start >= V)
        return result;

    vector<bool> visited(V, false);

    priority_queue<
        tuple<int, int, int>,
        vector<tuple<int, int, int> >,
        greater<tuple<int, int, int> >
    > pq;

    pq.push(make_tuple(0, start, -1));

    while (!pq.empty()) {

        tuple<int, int, int> top = pq.top();
        pq.pop();

        int weight  = get<0>(top);
        int current = get<1>(top);
        int parent  = get<2>(top);

        if (visited[current])
            continue;

        visited[current] = true;

        result.totalCost += weight;

        if (parent != -1) {

            MSTEdge e;
            e.from = parent;
            e.to = current;
            e.weight = weight;
            result.edges.push_back(e);
        }

        for (size_t i = 0; i < graph[current].size(); i++) {

            int neighbor   = graph[current][i].first;
            int edgeWeight = graph[current][i].second;

            if (!visited[neighbor])
                pq.push(make_tuple(edgeWeight, neighbor, current));
        }
    }

    result.connected =
        (static_cast<int>(result.edges.size()) == V - 1);

    return result;
}


void printMST(
    const MSTResult& result,
    const vector<string>& cities
) {
    cout << "\n";
    cout << "========================================\n";
    cout << "       DELIVERY NETWORK (MST)\n";
    cout << "========================================\n";

    for (size_t i = 0; i < result.edges.size(); i++) {

        const MSTEdge& edge = result.edges[i];

        cout
            << cities[edge.from]
            << " ---> "
            << cities[edge.to]
            << "   Cost: "
            << edge.weight
            << " km\n";
    }

    cout << "\n";
    cout << "Total Delivery Network Cost: "
         << result.totalCost
         << " km\n";

    if (!result.connected) {

        cout << "WARNING: network is disconnected, "
             << "some cities cannot be reached.\n";
    }
}
