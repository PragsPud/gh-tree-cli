#include "github_tree.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <owner> <repo> [branch]\n";
        return 1;
    }

    std::string owner = argv[1];
    std::string repo = argv[2];
    std::string token = std::getenv("GITHUB_TOKEN") ? std::getenv("GITHUB_TOKEN") : "";

    // NEW LOGIC: If no branch provided, look it up automatically
    std::string branch;
    if (argc == 4) {
        branch = argv[3];
    } else {
        std::cout << "[System] No branch specified. Detecting default branch...\n";
        branch = fetch_default_branch(owner, repo, token);
        std::cout << "[System] Detected default branch: " << branch << "\n";
    }

    auto tree = fetch_github_tree(owner, repo, branch, token);
    print_tree(tree, std::cout);

    return 0;
}