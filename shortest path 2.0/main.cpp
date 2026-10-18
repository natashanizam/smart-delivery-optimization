#include "Item.h"
#include "Graph.h"
#include "Knapsack.h"
#include "MST.h"
#include "Dijkstra.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;


void printHeader() {

    cout << "\n";
    cout << "============================================\n";
    cout << "      SMART DELIVERY OPTIMIZATION\n";
    cout << "============================================\n";
}

void printItems(const vector<Item>& items) {

    cout << "\n";
    cout << left
         << setw(15) << "Product"
         << setw(12) << "Weight"
         << setw(12) << "Value"
         << setw(12) << "Type"
         << "\n";

    cout << "---------------------------------------------\n";

    for (size_t i = 0; i < items.size(); i++) {

        cout << left
             << setw(15) << items[i].name
             << setw(12) << items[i].weight
             << setw(12) << items[i].value
             << setw(12)
             << (items[i].divisible ? "Divisible" : "0/1")
             << "\n";
    }
}


void loadDefaultData(
    vector<Item>& items,
    int& capacity,
    vector<string>& cities,
    Graph& graph
) {
    items.clear();

    Item laptop = {"Laptop", 3, 1000, false};
    Item rice   = {"Rice",  10,  200, true};
    Item phone  = {"Phone",  2,  700, false};
    Item oil    = {"Oil",    3,  300, true};
    Item wheat  = {"Wheat",  2,  300, true};

    items.push_back(laptop);
    items.push_back(rice);
    items.push_back(phone);
    items.push_back(oil);
    items.push_back(wheat);

    capacity = 10;

    cities.clear();
    cities.push_back("Warehouse");
    cities.push_back("City A");
    cities.push_back("City B");
    cities.push_back("City C");
    cities.push_back("City D");

    graph.assign(cities.size(), vector<pair<int, int> >());

    addEdge(graph, 0, 1, 2);   // Warehouse <-> City A
    addEdge(graph, 0, 3, 6);   // Warehouse <-> City C
    addEdge(graph, 1, 2, 3);   // City A    <-> City B
    addEdge(graph, 1, 3, 8);   // City A    <-> City C
    addEdge(graph, 1, 4, 5);   // City A    <-> City D
    addEdge(graph, 2, 4, 7);   // City B    <-> City D
    addEdge(graph, 3, 4, 9);   // City C    <-> City D
}



bool loadFromFile(
    const string& path,
    vector<Item>& items,
    int& capacity,
    vector<string>& cities,
    Graph& graph,
    string& error
) {
    ifstream in(path.c_str());

    if (!in) {
        error = "cannot open file '" + path + "'";
        return false;
    }

    string key;
    int nItems = 0, nCities = 0, nEdges = 0;

    if (!(in >> key >> capacity) || key != "capacity" || capacity < 0) {
        error = "expected 'capacity <non-negative int>'";
        return false;
    }

    if (!(in >> key >> nItems) || key != "items" || nItems < 0) {
        error = "expected 'items <count>'";
        return false;
    }

    items.clear();

    for (int i = 0; i < nItems; i++) {

        Item item;
        int divisible = 0;

        if (!(in >> item.name >> item.weight >> item.value >> divisible)) {
            error = "bad item line";
            return false;
        }

        if (item.weight <= 0 || item.value < 0) {
            error = "item '" + item.name +
                    "' needs weight > 0 and value >= 0";
            return false;
        }

        item.divisible = (divisible != 0);
        items.push_back(item);
    }

    if (!(in >> key >> nCities) || key != "cities" || nCities <= 0) {
        error = "expected 'cities <count>' with count > 0";
        return false;
    }

    in.ignore(numeric_limits<streamsize>::max(), '\n');

    cities.clear();

    for (int i = 0; i < nCities; i++) {

        string line;

        if (!getline(in, line)) {
            error = "missing city name";
            return false;
        }

        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);

        cities.push_back(line);
    }

    if (!(in >> key >> nEdges) || key != "edges" || nEdges < 0) {
        error = "expected 'edges <count>'";
        return false;
    }

    graph.assign(nCities, vector<pair<int, int> >());

    for (int i = 0; i < nEdges; i++) {

        int u, v, w;

        if (!(in >> u >> v >> w)) {
            error = "bad edge line";
            return false;
        }

        try {
            addEdge(graph, u, v, w);
        } catch (const exception& e) {
            error = e.what();
            return false;
        }
    }

    return true;
}


void printRoutes(
    const ShortestPathResult& sp,
    const vector<string>& cities
) {
    cout << "\n";
    cout << "============================================\n";
    cout << "     SHORTEST ROUTES FROM " << cities[0] << "\n";
    cout << "============================================\n";

    for (size_t c = 1; c < cities.size(); c++) {

        cout << left << setw(12) << cities[c] << ": ";

        vector<int> path = buildPath(sp, static_cast<int>(c));

        if (path.empty()) {
            cout << "unreachable\n";
            continue;
        }

        for (size_t i = 0; i < path.size(); i++) {

            if (i > 0)
                cout << " -> ";

            cout << cities[path[i]];
        }

        cout << "  (" << sp.dist[c] << " km)\n";
    }
}


int main(int argc, char* argv[]) {

    vector<Item> items;
    int capacity = 0;
    vector<string> cities;
    Graph graph;

    if (argc > 1) {

        string error;

        if (!loadFromFile(argv[1], items, capacity, cities, graph, error)) {
            cerr << "Input error: " << error << "\n";
            return 1;
        }

    } else {

        loadDefaultData(items, capacity, cities, graph);
    }

    printHeader();

    cout << "\nAvailable Products\n";

    printItems(items);

    vector<Item> divisibleItems;
    vector<Item> indivisibleItems;

    for (size_t i = 0; i < items.size(); i++) {

        if (items[i].divisible)
            divisibleItems.push_back(items[i]);
        else
            indivisibleItems.push_back(items[i]);
    }


    cout << "\n";
    cout << "============================================\n";
    cout << "             CARGO OPTIMIZATION\n";
    cout << "============================================\n";

    cout << "\nVehicle Capacity: " << capacity << " units\n";

    KnapsackResult result = knapsack01(indivisibleItems, capacity);

    int remainingCapacity = capacity - result.usedWeight;

    cout << "\n--- 0/1 Knapsack ---\n";
    cout << "Maximum Value: " << result.totalValue << "\n";
    cout << "Used Capacity: " << result.usedWeight
         << " / " << capacity << "\n";

    cout << "\nSelected Indivisible Items:\n";

    for (size_t i = 0; i < result.selectedItems.size(); i++)
        cout << "  - " << result.selectedItems[i] << "\n";

   
    vector<string> selectedDivisibleItems;

    double fractionalValue = fractionalKnapsack(
        divisibleItems,
        remainingCapacity,
        selectedDivisibleItems
    );

    cout << "\n--- Fractional Knapsack ---\n";

    cout << fixed << setprecision(2);

    cout << "Available Capacity: " << remainingCapacity << "\n";
    cout << "Additional Value: " << fractionalValue << "\n";

    cout << "\nSelected Divisible Items:\n";

    for (size_t i = 0; i < selectedDivisibleItems.size(); i++)
        cout << "  - " << selectedDivisibleItems[i] << "\n";

    MSTResult mst = primMST(graph, 0);

    printMST(mst, cities);

    ShortestPathResult sp = dijkstra(graph, 0);

    printRoutes(sp, cities);

    int farthest = 0;

    for (size_t c = 0; c < sp.dist.size(); c++) {

        if (sp.dist[c] != INF && sp.dist[c] > farthest)
            farthest = sp.dist[c];
    }

    double combinedValue = result.totalValue + fractionalValue;

    cout << "\n";
    cout << "============================================\n";
    cout << "                 SUMMARY\n";
    cout << "============================================\n";

    cout << "0/1 Knapsack Value       : " << result.totalValue << "\n";
    cout << "Fractional Value         : " << fractionalValue << "\n";
    cout << "Combined Cargo Value     : " << combinedValue << "\n";
    cout << "Delivery Network Cost    : " << mst.totalCost << " km\n";
    cout << "Farthest City Distance   : " << farthest << " km\n";

    if (farthest > 0) {

        cout << "Cargo Value per km (far) : "
             << combinedValue / farthest << "\n";
    }

    cout << "\nOptimization completed successfully.\n";

    return 0;
}
