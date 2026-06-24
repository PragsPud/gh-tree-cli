# gh-tree-cli

A high-performance, lightweight C++ utility to fetch and display the directory 
tree of a GitHub repository without cloning it.

## Features
- Single API call (recursive) for fast performance.
- Auto-detects repository default branch.
- Supports authenticated requests for higher API rate limits.
- Zero-dependency compilation (via system packages).

## Installation
Requires CMake, a C++17 compiler, and libcurl.

### Build (using Ninja)
1. mkdir build && cd build
2. cmake -DCMAKE_BUILD_TYPE=Release ..
3. ninja

## Usage
Basic usage:  
  ./gh_tree.exe <owner> <repo>

Authenticated usage:  
  export GITHUB_TOKEN="your_token_here"  
  ./gh_tree.exe <owner> <repo>

## Dependencies
- libcurl
- nlohmann-json
