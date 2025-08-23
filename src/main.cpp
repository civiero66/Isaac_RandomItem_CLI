#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <locale>
#include "http.h"
#include "parser.h"
#include "json.h"
#include "logic.h"
#include "cxxopts.hpp"

int main(int argc, char** argv) 
{
    setlocale(LC_ALL, "");

    cxxopts::Options options("IsaacRandomItem");

    options.add_options()
        ("h,help","Help")
        ("o,output","Output JSON file", cxxopts::value<std::string>()->default_value("items.json"))
        ("p,pool","Item pool filter", cxxopts::value<std::string>()->default_value(""))
        ("v,verbose","Verbose output", cxxopts::value<bool>()->default_value("false"))
        ("f,force","Force reload from website", cxxopts::value<bool>()->default_value("false"));

    auto result = options.parse(argc, argv);

    if (result.count("help")) 
    {
        std::cout << options.help() << "\n";
        return 0;
    }

    std::string output_file = result["output"].as<std::string>();
    std::string pool_filter = result["pool"].as<std::string>();
    bool verbose = result["verbose"].as<bool>();
    bool force_reload = result["force"].as<bool>();

    if (verbose) 
    {
        std::cout << "Output file: " << output_file << "\n";
        std::cout << "Pool filter: " << (pool_filter.empty() ? "<empty>" : pool_filter) << "\n";
        std::cout << "Force reload: " << (force_reload ? "true" : "false") << "\n";
    }

    std::vector<Item> items;

    if (force_reload) 
    {
        std::string html = HtmlRq("https://tboi.com/all-items");
        if (html.empty()) 
        {
            std::cerr << "Html is empty\n";
            return 1;
        }

        htmlDocPtr doc = htmlReadMemory(html.c_str(), html.size(), NULL, NULL,
            HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
        if (!doc) 
        {
            std::cerr << "htmlReadMemory return NULL\n";
            return 1;
        }

        items = ParseHtml(doc);
        if (doc) xmlFreeDoc(doc);

        SaveItemsToJson(output_file, items);
    }
    else 
    {
        items = LoadItemsFromJson(output_file);
        if (items.empty()) 
        {
            if (verbose) std::cout << "JSON is empty, making the download from the site\n";

            std::string html = HtmlRq("https://tboi.com/all-items");
            if (html.empty()) 
            {
                std::cerr << "HTML is empty\n";
                return 1;
            }

            htmlDocPtr doc = htmlReadMemory(html.c_str(), html.size(), NULL, NULL,
                HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
            if (!doc) 
            {
                std::cerr << "htmlReadMemory return NULL\n";
                return 1;
            }

            items = ParseHtml(doc);
            if (doc) xmlFreeDoc(doc);

            SaveItemsToJson(output_file, items);
        }
    }

    if (verbose) 
    {
        std::cout << "Total elements: " << items.size() << "\n";
    }

    if (pool_filter.empty()) 
    {
        std::cout << "Enter the pool (Item Room, Angel Room, Devil Room, Boss Room, Planetarium): ";
        std::getline(std::cin, pool_filter);
    }

    auto filtered_items = FilteredPool(items, pool_filter);
    if (filtered_items.empty()) 
    {
        std::cout << "Filter is empty, display all elements\n";
        filtered_items = items;
    }

    PrintRandomItem(filtered_items);

    xmlCleanupParser();
    return 0;
}