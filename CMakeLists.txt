cmake_minimum_required(VERSION 3.13)
project(dependency_tree)

set(CMAKE_CXX_STANDARD 14)

add_executable(dependency_tree main.cpp)
target_link_libraries(dependency_tree jsoncpp)