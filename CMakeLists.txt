PROJECT(ComponentTree)

cmake_minimum_required(VERSION 2.6)

find_package(Imagine REQUIRED)

add_executable(ComponentTree main.cpp component_tree.h component_tree.cpp)
ImagineUseModules(ComponentTree Graphics)

