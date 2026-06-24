#include "github_tree.hpp"
#include <iostream>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* mem = static_cast<std::string*>(userp);
    mem->append(static_cast<char*>(contents), realsize);
    return realsize;
}

std::string fetch_default_branch(const std::string& owner, const std::string& repo, const std::string& token) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    std::string default_branch = "main"; // Fallback

    if (curl) {
        std::string url = "https://api.github.com/repos/" + owner + "/" + repo;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Cpp-GitHub-Tree-App");
        if (!token.empty()) {
            std::string auth = "Authorization: Bearer " + token;
            headers = curl_slist_append(headers, auth.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        try {
            auto repo_info = json::parse(readBuffer);
            if (repo_info.contains("default_branch")) {
                default_branch = repo_info["default_branch"];
            }
        } catch (...) {}
    }
    return default_branch;
}

std::vector<TreeItem> fetch_github_tree(const std::string& owner, const std::string& repo, const std::string& branch, const std::string& token) {
    std::vector<TreeItem> tree_items;
    CURL* curl = curl_easy_init();
    CURLcode res;
    std::string readBuffer;

    if (curl) {
        std::string url = "https://api.github.com/repos/" + owner + "/" + repo + "/git/trees/" + branch + "?recursive=1";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Cpp-GitHub-Tree-App");
        headers = curl_slist_append(headers, "Accept: application/vnd.github.v3+json");
        
        if (!token.empty()) {
            std::string auth_header = "Authorization: Bearer " + token;
            headers = curl_slist_append(headers, auth_header.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Network Request Failed: " << curl_easy_strerror(res) << "\n";
            return tree_items;
        }

        try {
            auto json_response = json::parse(readBuffer);
            
            if (json_response.contains("message")) {
                std::cerr << "GitHub API Error: " << json_response["message"] << "\n";
                return tree_items;
            }

            if (json_response.contains("tree")) {
                for (const auto& item : json_response["tree"]) {
                    TreeItem ti;
                    ti.path = item["path"];
                    ti.type = item["type"];
                    tree_items.push_back(ti);
                }
            }
        } catch (json::parse_error& e) {
            std::cerr << "JSON Parsing Exception: " << e.what() << "\n";
        }
    }
    return tree_items;
}

void print_tree(const std::vector<TreeItem>& tree, std::ostream& out) {
    if (tree.empty()) {
        out << "Tree view is empty.\n";
        return;
    }

    for (const auto& item : tree) {
        int depth = std::count(item.path.begin(), item.path.end(), '/');
        
        std::string indent;
        for (int i = 0; i < depth; ++i) {
            indent += "|   "; // ASCII vertical bar
        }

        if (depth > 0) {
            indent += "|-- "; // ASCII branch
        }

        std::string basename = item.path.substr(item.path.find_last_of('/') + 1);
        // ASCII-only representation of types
        std::string label = (item.type == "tree") ? "[DIR]  " : "[FILE] ";

        out << indent << label << basename << "\n";
    }
}