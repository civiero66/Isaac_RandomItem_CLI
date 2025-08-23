#pragma once
#include <vector>
#include <string>
#include "item.h"

void SaveItemsToJson(const std::string& filename, const std::vector<Item>& items);
std::vector<Item> LoadItemsFromJson(const std::string& filename);