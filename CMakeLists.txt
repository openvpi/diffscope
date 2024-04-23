cmake_minimum_required(VERSION 3.19)

project(DiffScope VERSION 0.1.0.11 LANGUAGES CXX)

# ----------------------------------
# Build Options
# ----------------------------------
option(APPLICATION_ENABLE_DEVEL "Install developer files" OFF)
option(APPLICATION_INSTALL "Install this project" ON)
option(APPLICATION_BUILD_TESTS "Build test cases" OFF)

option(APPLICATION_ENABLE_BREAKPAD "Enable breakpad configuration" OFF)

# ----------------------------------
# CMake Settings
# ----------------------------------
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
else()
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
endif()

if(APPLICATION_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
endif()

# ----------------------------------
# Project Variables
# ----------------------------------
set(APPLICATION_VERSION ${PROJECT_VERSION})
set(APPLICATION_INSTALL_NAME ${PROJECT_NAME})

# ----------------------------------
# Find basic dependencies
# ----------------------------------
find_package(qmsetup REQUIRED)

qm_import(Filesystem)
qm_init_directories()

# ----------------------------------
# Add source modules
# ----------------------------------
add_subdirectory(libs)

add_subdirectory(src)