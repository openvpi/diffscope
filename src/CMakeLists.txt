# Add `ChorusKitAPI`
if(BUILD_LIBRARIES_IN_PLACE)
    include(../libs/choruskit/cmake/ChorusKitAPI.cmake)
else()
    find_package(ChorusKit REQUIRED)
endif()

ck_init_build_system(${PROJECT_NAME}
    ROOT ..
    VENDOR "OpenVPI"
)

if(BUILD_LIBRARIES_IN_PLACE)
    set(EXTERNAL_BUILD_MAIN_DIR ${CMAKE_BINARY_DIR}/out-Libraries)
    add_subdirectory(../libs ../libs)
endif()

# ----------------------------------
# Compile Definitions
# ----------------------------------
ck_add_definition(APP_NAME ${PROJECT_NAME})
ck_add_definition(APP_VERSION ${PROJECT_VERSION})

ck_add_definition(CONFIG_ENABLE_BREAKPAD ${DIFFSCOPE_ENABLE_BREAKPAD})

# ----------------------------------
# Main Project
# ----------------------------------
add_subdirectory(app)

add_subdirectory(libs)

# add_subdirectory(plugins)

# add_subdirectory(share)
ck_finish_build_system()