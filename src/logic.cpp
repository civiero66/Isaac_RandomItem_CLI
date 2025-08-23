#include "logic.h"
#include <iostream>
#include <random>

std::vector<Item> FilteredPool(const std::vector<Item>& items, const std::string& poolname) {
    if (poolname.empty()) return items;
    std::vector<Item> filtered;
    for (const auto& it : items) {
        if (it.itemPool.find(poolname) != std::string::npos) {
            filtered.push_back(it);
        }
    }
    return filtered;
}

void PrintRandomItem(const std::vector<Item>& items) {
    if (items.empty()) {
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, items.size() - 1);
    const Item& it = items[dist(gen)];
    std::cout << "ID: " << it.id << "\nName: " << it.name
        << "\nQuality: " << it.quality << "\nItem Pool: " << it.itemPool << "\n";
}