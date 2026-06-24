cmake_minimum_required(VERSION 3.14)
project(GitHubTree VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(include)

add_executable(gh_tree src/main.cpp src/github_tree.cpp)

target_link_libraries(gh_tree PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)