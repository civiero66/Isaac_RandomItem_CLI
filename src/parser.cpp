#include "parser.h"
#include <libxml/xpath.h>
#include <iostream>

std::vector<Item> ParseHtml(htmlDocPtr doc) 
{
    std::vector<Item> items;
    if (!doc) return items;

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (!context) return items;

    xmlXPathObjectPtr obj = xmlXPathEvalExpression(BAD_CAST "//li[@class='textbox']", context);
    if (!obj && !obj->nodesetval && obj->nodesetval->nodeNr == 0) 
    {
        if (obj) xmlXPathFreeObject(obj);
        xmlXPathFreeContext(context);
        return items;
    }

    for (int i = 0; i < obj->nodesetval->nodeNr; ++i) 
    {
        xmlNodePtr li = obj->nodesetval->nodeTab[i];
        if (!li) continue;

        Item it;

        xmlChar* sid = xmlGetProp(li, BAD_CAST "data-sid");
        if (sid) 
        {
            it.id = std::stoi(reinterpret_cast<char*>(sid));
            xmlFree(sid);
        } else it.id = 0;

        xmlXPathContextPtr liContext = xmlXPathNewContext(doc);
        if (!liContext) continue;
        liContext->node = li;

        xmlXPathObjectPtr nameObj = xmlXPathEvalExpression(BAD_CAST ".//p[@class='item-title']", liContext);
        if (nameObj && nameObj->nodesetval && nameObj->nodesetval->nodeNr > 0) 
        {
            xmlChar* content = xmlNodeGetContent(nameObj->nodesetval->nodeTab[0]);
            if (content) 
            {
                it.name = reinterpret_cast<char*>(content);
                xmlFree(content);
            }
        }
        if (nameObj) xmlXPathFreeObject(nameObj);

        xmlXPathObjectPtr qualityObj = xmlXPathEvalExpression(BAD_CAST ".//p[@class='quality']", liContext);
        if (qualityObj && qualityObj->nodesetval && qualityObj->nodesetval->nodeNr > 0) 
        {
            xmlChar* content = xmlNodeGetContent(qualityObj->nodesetval->nodeTab[0]);
            if (content) 
            {
                std::string qstr(reinterpret_cast<char*>(content));
                auto pos = qstr.find(":");
                if (pos != std::string::npos)
                    it.quality = std::stoi(qstr.substr(pos + 1));
                xmlFree(content);
            }
        }
        if (qualityObj) xmlXPathFreeObject(qualityObj);

        xmlXPathObjectPtr poolObj = xmlXPathEvalExpression(BAD_CAST ".//ul/p[starts-with(text(),'Item Pool:')]", liContext);
        if (poolObj && poolObj->nodesetval && poolObj->nodesetval->nodeNr > 0) 
        {
            xmlChar* content = xmlNodeGetContent(poolObj->nodesetval->nodeTab[0]);
            if (content) 
            {
                std::string pool(reinterpret_cast<char*>(content));
                auto pos = pool.find(":");
                if (pos != std::string::npos)
                    it.itemPool = pool.substr(pos + 2);
                xmlFree(content);
            }
        }
        if (poolObj) xmlXPathFreeObject(poolObj);

        xmlXPathFreeContext(liContext);

        if (!it.itemPool.empty()) items.push_back(it);
    }

    xmlXPathFreeObject(obj);
    xmlXPathFreeContext(context);

    return items;
}