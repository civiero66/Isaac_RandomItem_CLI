#include "json.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void to_json(json& j, const Item& it) {
    j = json{ {"id", it.id}, {"name", it.name}, {"quality", it.quality}, {"ItemPool", it.itemPool} };
}

void from_json(const json& j, Item& it) {
    j.at("id").get_to(it.id);
    j.at("name").get_to(it.name);
    j.at("quality").get_to(it.quality);
    j.at("ItemPool").get_to(it.itemPool);
}

void SaveItemsToJson(const std::string& filename, const std::vector<Item>& items) {
    std::ofstream save_file(filename);
    json j = items;
    save_file << j.dump(4);
}

std::vector<Item> LoadItemsFromJson(const std::string& filename) 
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) 
    {
        std::cerr << "Couln't open the file: " << filename << std::endl;
        return {};
    }

    std::string content((std::istreambuf_iterator<char>(in)),
                        (std::istreambuf_iterator<char>()));
    return nlohmann::json::parse(content).get<std::vector<Item>>();
}