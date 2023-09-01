cmake_minimum_required(VERSION 3.17)

project(DiffScope
    VERSION 0.0.1.1
    LANGUAGES CXX
)

# Build options
option(DIFFSCOPE_ENABLE_DEVEL "Develop mode" off)
option(DIFFSCOPE_ENABLE_BREAKPAD "Use breakpad handler" off)
option(DIFFSCOPE_BUILD_TESTS "Build test cases" on)

# Develop options
#   BUILD_LIBRARIES_IN_PLACE

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

add_subdirectory(src)