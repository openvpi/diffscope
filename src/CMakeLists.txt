# Add `ChorusKitAPI`
if(NOT TARGET ChorusKit::AppCore)
    find_package(ChorusKit REQUIRED)
endif()

ck_init_build_system()

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
if(DIFFSCOPE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

ck_finish_build_system()