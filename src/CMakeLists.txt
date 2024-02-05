# Add `ChorusKitAPI`
if(NOT TARGET ChorusKit::Loader)
    find_package(ChorusKit REQUIRED)
endif()

include(${CK_CMAKE_MODULES_DIR}/ChorusKitAPI.cmake)

# ----------------------------------
# Project Variables
# ----------------------------------
set(CK_APPLICATION_NAME ${PROJECT_NAME})
set(CK_APPLICATION_DESCRIPTION ${PROJECT_NAME})
set(CK_APPLICATION_VERSION ${PROJECT_VERSION})
set(CK_APPLICATION_VENDOR OpenVPI)
set(CK_DEV_START_YEAR 2019)

# ----------------------------------
# Initializations
# ----------------------------------
ck_init_buildsystem()

# ----------------------------------
# Compile Definitions
# ----------------------------------
qm_add_definition(APP_NAME ${PROJECT_NAME} STRING_LITERAL)
qm_add_definition(APP_VERSION ${PROJECT_VERSION} STRING_LITERAL)
qm_add_definition(CONFIG_ENABLE_BREAKPAD CONDITION DIFFSCOPE_ENABLE_BREAKPAD)

# ----------------------------------
# Main Project
# ----------------------------------
add_subdirectory(app)

add_subdirectory(libs)

add_subdirectory(plugins)

add_subdirectory(share)

# ----------------------------------
# Finale
# ----------------------------------
ck_finish_buildsystem()