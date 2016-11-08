PROJECT(libmdp)
cmake_minimum_required(VERSION 2.8)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${SOURCE_DIR}")

find_package(libzmq REQUIRED)
IF (LIBZMQ_FOUND)
    include_directories(${LIBZMQ_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${LIBZMQ_LIBRARIES})
ELSE (LIBZMQ_FOUND)
    message( FATAL_ERROR "libzmq not found." )
ENDIF (LIBZMQ_FOUND)

include_directories("${SOURCE_DIR}/include")
aux_source_directory("${SOURCE_DIR}/src" SRC_LIST)
add_library(libmdp SHARED ${SRC_LIST})
target_link_libraries(libmdp $(LIBZMQ_LIBRARIES))

