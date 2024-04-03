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
    # Install data files
    install(DIRECTORY ${CK_BUILD_MAIN_DIR}/share/
        DESTINATION ${CK_INSTALL_SHARE_DIR}
    )

    # Prevent CMake from removing rpaths when install
    if(NOT WIN32)
        set(_marker_file ${CMAKE_BINARY_DIR}/pre_install_timestamp)

        if(EXISTS ${_marker_file})
            file(REMOVE ${_marker_file})
        endif()

        if(APPLE)
            set(_command sed -i '' -E '/[[:space:]]*delete_rpath[[:space:]]+\".+\"/d' {})
        else()
            set(_command _)
        endif()

        add_custom_command(
            OUTPUT ${_marker_file}
            COMMAND find "${CMAKE_BINARY_DIR}" -type f -name cmake_install.cmake -exec ${_command} \\\\;
            COMMAND ${CMAKE_COMMAND} -E touch ${_marker_file}
            COMMENT "Running patch on all \"cmake_install.cmake\" files"
        )

        add_custom_target(
            pre_install ALL DEPENDS ${_marker_file}
        )
    endif()

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
        list(APPEND _plugins
            platforms/qcocoa
            styles/qmacstyle
            platforminputcontexts/q*
            printsupport/q*
            virtualkeyboard/q*
        )
    else()
        # Linux
        set(_)
    endif()

    if(WIN32)
        set(_lib_dir ${CK_INSTALL_RUNTIME_DIR})
    else()
        set(_lib_dir ${CK_INSTALL_LIBRARY_DIR})
    endif()

    qm_deploy_directory(${CMAKE_INSTALL_PREFIX}
        PLUGINS ${_plugins}
        LIBRARY_DIR ${_lib_dir}
        PLUGIN_DIR ${CK_INSTALL_LIBRARY_DIR}/Qt/plugins
        WIN_SEARCHING_PATHS ${CK_BUILD_RUNTIME_DIR}
        VERBOSE
    )
endif()