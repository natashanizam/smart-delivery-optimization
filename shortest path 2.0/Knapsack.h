#ifndef KNAPSACK_H
#define KNAPSACK_H

#include "Item.h"
#include <vector>
#include <string>

struct KnapsackResult {
    int totalValue;
    int usedWeight;
    std::vector<std::string> selectedItems;
};

KnapsackResult knapsack01(
    const std::vector<Item>& items,
    int capacity
);

double fractionalKnapsack(
    std::vector<Item> items,
    int capacity,
    std::vector<std::string>& selectedItems
);

#endif
