#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct Item {
    int id;
    std::string name;
    int quality;
    std::string itemPool;
};

void to_json(nlohmann::json& j, const Item& it);
void from_json(const nlohmann::json& j, Item& it);