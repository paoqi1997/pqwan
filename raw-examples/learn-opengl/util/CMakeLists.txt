cmake_minimum_required(VERSION 3.4)
project(libutil)

get_filename_component(ROOT_DIR .. ABSOLUTE)

set(INCS_DIR ${ROOT_DIR}/3rd-party/include)
set(LIBS_DIR ${ROOT_DIR}/3rd-party/lib)

include_directories(${INCS_DIR})
link_directories(${LIBS_DIR})

set(LIBS glad glfw3 opengl32)
set(BIN app)
set(LIB_STATIC util)

file(GLOB UTIL_SRCS util.cpp)
file(GLOB UTIL_INCS util.h)
file(GLOB TEST test.cpp)

source_group("src" FILES ${UTIL_SRCS})
source_group("include" FILES ${UTIL_INCS})
source_group("test" FILES ${TEST})

if(WIN32)
    add_executable(${BIN} ${TEST} ${UTIL_SRCS} ${UTIL_INCS})
    target_link_libraries(${BIN} ${LIBS})

    add_library(${LIB_STATIC} STATIC ${UTIL_SRCS} ${UTIL_INCS})
    target_link_libraries(${LIB_STATIC} ${LIBS})

    # 拷贝头文件到目标目录中
    add_custom_command(
        TARGET ${LIB_STATIC} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy ${UTIL_INCS} ${INCS_DIR}
        VERBATIM
    )

    add_custom_command(
        TARGET ${LIB_STATIC} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "${UTIL_INCS} -> ${INCS_DIR}"
        VERBATIM
    )

    # 拷贝静态库到目标目录中
    add_custom_command(
        TARGET ${LIB_STATIC} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE_DIR:${LIB_STATIC}>/${LIB_STATIC}.lib ${LIBS_DIR}
        VERBATIM
    )

    add_custom_command(
        TARGET ${LIB_STATIC} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "$<TARGET_FILE_DIR:${LIB_STATIC}>/${LIB_STATIC}.lib -> ${LIBS_DIR}"
        VERBATIM
    )
endif()
