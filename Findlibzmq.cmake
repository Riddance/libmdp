include(FindPkgConfig)
pkg_check_modules(PC_LIBZMQ "libzmq")
if (NOT PC_LIBZMQ_FOUND)
    pkg_check_modules(PC_LIBZMQ "libzmq")
endif (NOT PC_LIBZMQ_FOUND)
if (PC_LIBZMQ_FOUND)
    set(PC_LIBZMQ_INCLUDE_HINTS ${PC_LIBZMQ_INCLUDE_DIRS} ${PC_LIBZMQ_INCLUDE_DIRS}/*)
    set(PC_LIBZMQ_LIBRARY_HINTS ${PC_LIBZMQ_LIBRARY_DIRS} ${PC_LIBZMQ_LIBRARY_DIRS}/*)
endif(PC_LIBZMQ_FOUND)

find_path (
    LIBZMQ_INCLUDE_DIRS
    NAMES zmq.h
    HINTS ${PC_LIBZMQ_INCLUDE_HINTS}
)

find_library (
    LIBZMQ_LIBRARIES
    NAMES zmq
    HINTS ${PC_LIBZMQ_LIBRARY_HINTS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LIBZMQ
    REQUIRED_VARS LIBZMQ_LIBRARIES LIBZMQ_INCLUDE_DIRS
)
mark_as_advanced(
    LIBZMQ_FOUND
    LIBZMQ_LIBRARIES LIBZMQ_INCLUDE_DIRS
)
