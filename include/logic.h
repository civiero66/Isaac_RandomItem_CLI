#pragma once
#include <vector>
#include "item.h"

std::vector<Item> FilteredPool(const std::vector<Item>& items, const std::string& poolname);
void PrintRandomItem(const std::vector<Item>& items);