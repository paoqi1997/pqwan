cmake_minimum_required(VERSION 3.4)
project(color)

set(INCS_DIR ../../3rd-party/include)
set(LIBS_DIR ../../3rd-party/lib)

include_directories(${INCS_DIR})
link_directories(${LIBS_DIR})

set(LIBS util glad glfw3 opengl32)
set(SRCS main.cpp)
set(BIN app)

if(WIN32)
    add_executable(${BIN} ${SRCS})
    target_link_libraries(${BIN} ${LIBS})
endif()
