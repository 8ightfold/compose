cmake_minimum_required(VERSION 3.23)

add_library(compose_includes INTERFACE)
target_include_directories(compose_includes INTERFACE include)
add_library(compose::compose ALIAS compose_includes)