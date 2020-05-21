cmake_minimum_required (VERSION 3.4)
project (libutil)

file (GLOB UTIL_SRCS util.cpp)
file (GLOB UTIL_INCS util.h)
file (GLOB TEST test.cpp)

source_group ("src" FILES ${UTIL_SRCS})
source_group ("include" FILES ${UTIL_INCS})
source_group ("test" FILES ${TEST})

if (WIN32)
    add_executable (app ${TEST} ${UTIL_SRCS} ${UTIL_INCS})
    add_library (util STATIC ${UTIL_SRCS} ${UTIL_INCS})
endif ()
