PROJECT(libmdp)
cmake_minimum_required(VERSION 2.8)

# Add common source
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")


add_library(libmdp SHARED tinyxml2.cpp tinyxml2.h)
set_target_properties()
