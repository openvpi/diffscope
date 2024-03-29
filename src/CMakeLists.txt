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
set(CK_APPLICATION_VENDOR "Team OpenVPI")

set(CK_DEV_START_YEAR 2019)

set(CK_BUILDINFO_PREFIX APPLICATION)

if(APPLICATION_ENABLE_DEVEL)
    set(CK_ENABLE_DEVEL on)
endif()

if(NOT APPLICATION_INSTALL)
    set(CK_ENABLE_INSTALL off)
endif()

# ----------------------------------
# Initializations
# ----------------------------------
ck_init_buildsystem()

# ----------------------------------
# Compile Definitions
# ----------------------------------
qm_add_definition(APP_NAME ${PROJECT_NAME} STRING_LITERAL)
qm_add_definition(APP_VERSION ${PROJECT_VERSION} STRING_LITERAL)
qm_add_definition(APP_ORG_NAME "OpenVPI" STRING_LITERAL)
qm_add_definition(APP_ORG_DOMAIN "org.openvpi" STRING_LITERAL)
qm_add_definition(APP_PLUGIN_IID "org.OpenVPI.${PROJECT_NAME}.Plugin" STRING_LITERAL)
qm_add_definition(CONFIG_ENABLE_BREAKPAD CONDITION APPLICATION_ENABLE_BREAKPAD)

# ----------------------------------
# Main Project
# ----------------------------------
add_subdirectory(app)

add_subdirectory(libs)

add_subdirectory(plugins)

add_subdirectory(share)

if(APPLICATION_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# ----------------------------------
# Finale
# ----------------------------------
ck_finish_buildsystem()

# Deploy
if(APPLICATION_INSTALL)
    qm_find_qt(Core)

    set(_plugins)

    if(QT_VERSION_MAJOR EQUAL 5)
        # Qt5
        list(APPEND _plugins
            bearer/qgenericbearer
            iconengines/qsvgicon
            imageformats/q*
        )
    else()
        # Qt6
        set(_)
    endif()

    if(WIN32)
        # Windows
        list(APPEND _plugins
            platforms/qwindows
            styles/qwindowsvistastyle
        )
    elseif(APPLE)
        # Mac
        set(_)
    else()
        # Linux
        set(_)
    endif()

    qm_deploy_directory(${CMAKE_INSTALL_PREFIX}
        PLUGINS ${_plugins}
        PLUGIN_DIR lib/Qt/plugins
        WIN_SEARCHING_PATHS ${CK_BUILD_RUNTIME_DIR}
        VERBOSE
    )
endif()