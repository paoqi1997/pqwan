cmake_minimum_required(VERSION 3.4)
project(tiles)

# https://cmake.org/cmake/help/latest/command/get_filename_component.html
get_filename_component(ROOT_DIR .. ABSOLUTE)

set(SDL3_DIR ${ROOT_DIR}/SDL3-3.2.16)
set(INCS_DIR ${SDL3_DIR}/include)
set(LIBS_DIR ${SDL3_DIR}/build/Debug)

include_directories(${INCS_DIR})
link_directories(${LIBS_DIR})

set(LIBS SDL3)
set(SRCS main.cpp)
set(BIN app)

if(WIN32)
    add_executable(${BIN} ${SRCS})
    target_link_libraries(${BIN} ${LIBS})

    # https://cmake.org/cmake/help/latest/command/add_custom_command.html
    add_custom_command(
        TARGET ${BIN} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${LIBS_DIR}/SDL3.dll" $<TARGET_FILE_DIR:${BIN}>
        VERBATIM
    )
endif()
