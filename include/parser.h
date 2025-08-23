#pragma once
#include <vector>
#include "item.h"
#include <libxml/HTMLparser.h>

std::vector<Item> ParseHtml(htmlDocPtr doc);