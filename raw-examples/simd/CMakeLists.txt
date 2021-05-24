cmake_minimum_required(VERSION 3.4)
project(simd)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-g)
endif()

file(GLOB SRCS *.cpp)
file(GLOB INCS *.h)

# 生成IDE工程时为源文件定义分组
source_group("src" FILES ${SRCS})
source_group("include" FILES ${INCS})

add_executable(app ${SRCS} ${INCS})
