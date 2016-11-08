PROJECT(libmdp)
cmake_minimum_required(VERSION 2.8)

find_package(libzmq REQUIRED)
IF (LIBZMQ_FOUND)
    include_directories(${LIBZMQ_INCLUDE_DIRS})
    list(APPEND MORE_LIBRARIES ${LIBZMQ_LIBRARIES})
ELSE (LIBZMQ_FOUND)
    message( FATAL_ERROR "libzmq not found." )
ENDIF (LIBZMQ_FOUND)

include_directories(./include)
aux_source_directory(./src SRC_LIST)
add_library(libmdp SHARED ${SRC_LIST})
target_link_libraries(libmdp $(ZEROMQ_LIBRARIES))

