cmake_minimum_required(VERSION 3.10)
project(path-indexer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/path_indexer)

add_library(path_indexer_core
        src/PathParser.cpp
)