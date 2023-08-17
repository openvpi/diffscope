cmake_minimum_required(VERSION 3.17)

project(DiffScope
    VERSION 0.0.1.1
    LANGUAGES CXX
)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

if(BUILD_LIBRARIES_IN_PLACE)
    add_subdirectory(libs)
    
    include(libs/choruskit/cmake/ChorusKitAPI.cmake)
else()
    find_package(ChorusKit REQUIRED)
endif()

add_subdirectory(src)