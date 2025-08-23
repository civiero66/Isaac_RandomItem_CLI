#include "http.h"
#include <curl/curl.h>
#include <iostream>

size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userd) {
    std::string* buffer = static_cast<std::string*>(userd);
    size_t totalSize = size * nmemb;
    buffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string HtmlRq(const std::string& url) {
    CURL* curl;
    CURLcode result;
    std::string html;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://tboi.com/all-items");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        result = curl_easy_perform(curl);

        if (result != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return html;
}