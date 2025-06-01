cmake_minimum_required (VERSION 3.4)
project (coordinate)

include_directories (../../3rd-party/include)
link_directories (../../3rd-party/lib)

set (LIBS util glad glfw3 opengl32)
set (SRCS main.cpp)

if (WIN32)
    add_executable (app ${SRCS})
    target_link_libraries (app ${LIBS})
endif ()
