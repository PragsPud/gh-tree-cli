#ifndef GITHUB_TREE_HPP
#define GITHUB_TREE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <ostream>

struct TreeItem {
    std::string path;
    std::string type; // "blob" for file, "tree" for folder
};

// Fetches the entire repository structure recursively in a single network call
std::vector<TreeItem> fetch_github_tree(const std::string& owner, 
                                        const std::string& repo, 
                                        const std::string& branch, 
                                        const std::string& token = "");

std::string fetch_default_branch(const std::string& owner, 
                                 const std::string& repo, 
                                 const std::string& token = "");

// Optimized tree printing layout
void print_tree(const std::vector<TreeItem>& tree, std::ostream& out = std::cout);

#endif // GITHUB_TREE_HPP