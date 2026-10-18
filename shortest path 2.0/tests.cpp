#include "Item.h"
#include "Graph.h"
#include "Knapsack.h"
#include "MST.h"
#include "Dijkstra.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;


static Item makeItem(const string& name, int w, int v, bool div) {
    Item it;
    it.name = name;
    it.weight = w;
    it.value = v;
    it.divisible = div;
    return it;
}

void testKnapsack() {

    vector<Item> items;
    items.push_back(makeItem("Laptop", 3, 1000, false));
    items.push_back(makeItem("Phone", 2, 700, false));
    items.push_back(makeItem("Book", 4, 300, false));

    KnapsackResult r = knapsack01(items, 5);

    assert(r.totalValue == 1700);
    assert(r.usedWeight == 5);
    assert(r.selectedItems.size() == 2);

    cout << "Knapsack test passed.\n";
}

void testKnapsackEdgeCases() {

    vector<Item> items;
    items.push_back(makeItem("Laptop", 3, 1000, false));

    assert(knapsack01(items, 0).totalValue == 0);
    assert(knapsack01(items, -5).totalValue == 0);

    vector<Item> none;
    KnapsackResult e = knapsack01(none, 10);
    assert(e.totalValue == 0 && e.selectedItems.empty());

    assert(knapsack01(items, 2).totalValue == 0);
    assert(knapsack01(items, 2).selectedItems.empty());

  
    KnapsackResult exact = knapsack01(items, 3);
    assert(exact.totalValue == 1000 && exact.usedWeight == 3);

    vector<Item> bad;
    bad.push_back(makeItem("Zero", 0, 500, false));
    bad.push_back(makeItem("Neg", -2, 500, false));
    bad.push_back(makeItem("Good", 2, 100, false));
    KnapsackResult b = knapsack01(bad, 5);
    assert(b.totalValue == 100);
    assert(b.selectedItems.size() == 1 && b.selectedItems[0] == "Good");

    cout << "Knapsack edge-case tests passed.\n";
}


void testFractionalKnapsack() {

    vector<Item> items;
    items.push_back(makeItem("Oil", 3, 300, true));
    items.push_back(makeItem("Wheat", 2, 300, true));

    vector<string> selected;

    double result = fractionalKnapsack(items, 2, selected);

    assert(fabs(result - 300.0) < 1e-9);

    cout << "Fractional Knapsack test passed.\n";
}

void testFractionalEdgeCases() {

    vector<Item> items;
    items.push_back(makeItem("Rice", 10, 200, true));

    vector<string> sel;
    double v = fractionalKnapsack(items, 5, sel);
    assert(fabs(v - 100.0) < 1e-9);
    assert(sel.size() == 1 && sel[0] == "Rice (partial)");

    vector<string> sel0;
    assert(fractionalKnapsack(items, 0, sel0) == 0.0);
    assert(sel0.empty());

    vector<Item> none;
    vector<string> sel1;
    assert(fractionalKnapsack(none, 10, sel1) == 0.0);

    vector<Item> bad;
    bad.push_back(makeItem("Zero", 0, 999, true));
    bad.push_back(makeItem("Ok", 2, 40, true));
    vector<string> sel2;
    assert(fabs(fractionalKnapsack(bad, 5, sel2) - 40.0) < 1e-9);

    vector<Item> mix;
    mix.push_back(makeItem("Sand", 5, 5, true));
    mix.push_back(makeItem("Gold", 1, 100, true));
    vector<string> sel3;
    double m = fractionalKnapsack(mix, 3, sel3);
    assert(fabs(m - 102.0) < 1e-9);  

    cout << "Fractional edge-case tests passed.\n";
}



void testMST() {

    Graph graph(4);

    addEdge(graph, 0, 1, 1);
    addEdge(graph, 0, 2, 4);
    addEdge(graph, 1, 2, 2);
    addEdge(graph, 1, 3, 5);
    addEdge(graph, 2, 3, 1);

    MSTResult result = primMST(graph, 0);

    assert(result.totalCost == 4);
    assert(result.edges.size() == 3);
    assert(result.connected);

    assert(primMST(graph, 3).totalCost == 4);

    cout << "Prim's MST test passed.\n";
}

void testMSTEdgeCases() {

    Graph one(1);
    MSTResult r1 = primMST(one, 0);
    assert(r1.totalCost == 0 && r1.edges.empty() && r1.connected);

    Graph dis(3);
    addEdge(dis, 0, 1, 7);
    MSTResult r2 = primMST(dis, 0);
    assert(r2.totalCost == 7);
    assert(r2.edges.size() == 1);
    assert(!r2.connected);

  
    MSTResult r3 = primMST(dis, 9);
    assert(r3.totalCost == 0 && r3.edges.empty());

    Graph empty;
    assert(primMST(empty, 0).totalCost == 0);

    cout << "Prim's MST edge-case tests passed.\n";
}

void testDijkstra() {

  
    Graph g(5);
    addEdge(g, 0, 1, 2);
    addEdge(g, 0, 3, 6);
    addEdge(g, 1, 2, 3);
    addEdge(g, 1, 3, 8);
    addEdge(g, 1, 4, 5);
    addEdge(g, 2, 4, 7);
    addEdge(g, 3, 4, 9);

    ShortestPathResult sp = dijkstra(g, 0);

    assert(sp.dist[0] == 0);
    assert(sp.dist[1] == 2);
    assert(sp.dist[2] == 5);
    assert(sp.dist[3] == 6);   // direct 6 beats 0-1-3 = 10
    assert(sp.dist[4] == 7);   // 0-1-4

    vector<int> path = buildPath(sp, 4);
    assert(path.size() == 3);
    assert(path[0] == 0 && path[1] == 1 && path[2] == 4);

    cout << "Dijkstra test passed.\n";
}

void testDijkstraEdgeCases() {

    // unreachable vertex
    Graph g(3);
    addEdge(g, 0, 1, 4);

    ShortestPathResult sp = dijkstra(g, 0);
    assert(sp.dist[2] == INF);
    assert(buildPath(sp, 2).empty());

    vector<int> self = buildPath(sp, 0);
    assert(self.size() == 1 && self[0] == 0);


    assert(buildPath(sp, 7).empty());
    assert(buildPath(sp, -1).empty());

   
    ShortestPathResult bad = dijkstra(g, 5);
    assert(bad.dist[0] == INF);

  
    Graph z(2);
    addEdge(z, 0, 1, 0);
    assert(dijkstra(z, 0).dist[1] == 0);

    cout << "Dijkstra edge-case tests passed.\n";
}


void testGraphValidation() {

    Graph g(2);

    bool negativeThrew = false;
    try { addEdge(g, 0, 1, -3); }
    catch (const invalid_argument&) { negativeThrew = true; }
    assert(negativeThrew);

    bool rangeThrew = false;
    try { addEdge(g, 0, 5, 1); }
    catch (const out_of_range&) { rangeThrew = true; }
    assert(rangeThrew);

    // nothing was added by the failed calls
    assert(g[0].empty() && g[1].empty());

    cout << "Graph validation tests passed.\n";
}


int main() {

    cout << "\nRunning tests...\n\n";

    testKnapsack();
    testKnapsackEdgeCases();

    testFractionalKnapsack();
    testFractionalEdgeCases();

    testMST();
    testMSTEdgeCases();

    testDijkstra();
    testDijkstraEdgeCases();

    testGraphValidation();

    cout << "\nAll tests passed successfully!\n";

    return 0;
}
