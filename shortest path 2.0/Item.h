#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    std::string name;
    int weight;
    int value;
    bool divisible;
};

#endif
