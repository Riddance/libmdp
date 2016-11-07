PROJECT(libmdp)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ./include
    ./libzmq/include
)
aux_source_directory(./src SRC_LIST)
link_directories(
    ./libzmq/lib
)

add_library(libmdp SHARED ${SRC_LIST})
