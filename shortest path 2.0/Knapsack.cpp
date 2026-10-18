#include "Knapsack.h"

#include <algorithm>
#include <vector>
#include <string>

using namespace std;


static bool isValid(const Item& item) {
    return item.weight > 0 && item.value >= 0;
}


KnapsackResult knapsack01(
    const vector<Item>& items,
    int capacity
) {
    KnapsackResult result;
    result.totalValue = 0;
    result.usedWeight = 0;

    if (capacity <= 0 || items.empty())
        return result;

    int n = static_cast<int>(items.size());

    vector<vector<int> > dp(
        n + 1,
        vector<int>(capacity + 1, 0)
    );

    for (int i = 1; i <= n; i++) {

        const Item& item = items[i - 1];

        for (int w = 1; w <= capacity; w++) {

            dp[i][w] = dp[i - 1][w];

            if (isValid(item) && item.weight <= w) {

                dp[i][w] = max(
                    dp[i][w],
                    item.value + dp[i - 1][w - item.weight]
                );
            }
        }
    }

    int w = capacity;

    for (int i = n; i >= 1; i--) {

        if (dp[i][w] != dp[i - 1][w]) {

            result.selectedItems.push_back(items[i - 1].name);

            result.usedWeight += items[i - 1].weight;

            w -= items[i - 1].weight;
        }
    }

    reverse(
        result.selectedItems.begin(),
        result.selectedItems.end()
    );

    result.totalValue = dp[n][capacity];

    return result;
}


static bool compareRatio(const Item& a, const Item& b) {
    return static_cast<long long>(a.value) * b.weight >
           static_cast<long long>(b.value) * a.weight;
}


double fractionalKnapsack(
    vector<Item> items,
    int capacity,
    vector<string>& selectedItems
) {
    if (capacity <= 0)
        return 0.0;

    vector<Item> valid;

    for (size_t i = 0; i < items.size(); i++) {

        if (isValid(items[i]))
            valid.push_back(items[i]);
    }

    sort(valid.begin(), valid.end(), compareRatio);

    double totalValue = 0.0;

    for (size_t i = 0; i < valid.size(); i++) {

        if (capacity == 0)
            break;

        const Item& item = valid[i];

        if (item.weight <= capacity) {

            capacity -= item.weight;

            totalValue += item.value;

            selectedItems.push_back(item.name);
        }

    
        else {

            double fraction =
                static_cast<double>(capacity) / item.weight;

            totalValue += item.value * fraction;

            selectedItems.push_back(item.name + " (partial)");

            capacity = 0;
        }
    }

    return totalValue;
}
