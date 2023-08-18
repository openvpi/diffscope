ck_configure_application(
    ICO app.ico
    ICNS app.icns
    NAME ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "DiffSinger Editor"
)

ck_target_components(${PROJECT_NAME}
    SOURCES main.cpp
    QT_LINKS Core Widgets
    LINKS_PRIVATE CkLoader CkAppCore qBreakpad
    INCLUDE_PRIVATE ${CK_BUILD_INCLUDE_DIR}
)

# Set configration file variables
set(APP_CONFIG_SPLASH_IMAGE "splash.svg")
set(APP_CONFIG_PADDING 20)

# Configure configuration file
set(_config_file ${CMAKE_CURRENT_BINARY_DIR}/config.json)
configure_file(config.json.in ${_config_file} @ONLY)

if(APPLE)
    set(_app_res_dir .)
    set(_app_doc_dir doc)
else()
    set(_app_res_dir ${PROJECT_NAME})
    set(_app_doc_dir doc/${PROJECT_NAME})
endif()

# Deploy docs
ck_add_shared_files(
    SKIP_BUILD
    SRC share/doc/* DEST ${_app_doc_dir}
)

# Deploy resources
ck_add_shared_files(
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST ${_app_res_dir}
)

# Deploy config files
ck_add_shared_files(
    SRC conf/${CK_PLATFORM_LOWER}/* DEST ${CK_BUILD_QT_CONF_DIR}
)

if(WIN32)
    # Deploy windows icons
    ck_add_shared_files(
        SKIP_BUILD
        SRC share/win-icons/* DEST ${PROJECT_NAME}/icons
    )

    # Add post build events to deploy qt libraries for Windows
    set(_deploy_runtime_target ${PROJECT_NAME})

    # add_custom_target(${_deploy_runtime_target} DEPENDS ${PROJECT_NAME})
    ck_add_qt_module(_qt_libs Core Gui Widgets Svg Concurrent Qml)
    set(_files)

    foreach(_item ${_qt_libs})
        get_target_property(_path ${_item} LOCATION)
        list(APPEND _files ${_path})
    endforeach()

    add_custom_command(TARGET ${_deploy_runtime_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_files} ${CK_BUILD_RUNTIME_DIR}
    )

    # Run windeployqt
    if(TARGET Qt${QT_VERSION_MAJOR}::qmake)
        if(NOT DEFINED QT_QMAKE_EXECUTABLE)
            get_target_property(QT_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
        endif()

        if(EXISTS "${QT_QMAKE_EXECUTABLE}")
            message(STATUS "Qmake found: ${QT_QMAKE_EXECUTABLE}")

            get_filename_component(_dir ${QT_QMAKE_EXECUTABLE} DIRECTORY)
            find_program(_windeploy NAMES windeployqt ${_dir})

            if(_windeploy)
                add_custom_command(TARGET ${_deploy_runtime_target} POST_BUILD
                    COMMAND set "Path=${_dir}\;%Path%\;"
                    COMMAND ${_windeploy}
                    --no-translations
                    --no-system-d3d-compiler
                    --no-virtualkeyboard
                    --no-compiler-runtime
                    --no-opengl-sw
                    --libdir "${CK_BUILD_RUNTIME_DIR}"
                    --plugindir "${CK_BUILD_LIBRARY_DIR}/Qt/plugins"
                    --verbose 0
                    $<TARGET_FILE:${PROJECT_NAME}>
                    WORKING_DIRECTORY ${CK_BUILD_RUNTIME_DIR}
                    COMMENT "Run windeployqt"
                    ENV
                )
            endif()
        else()
            set(QT_QMAKE_EXECUTABLE)
            message(WARNING "Qmake not found")
        endif()
    endif()
elseif(APPLE)
# TODO...
else()
    # Add linux files
    ck_add_shared_files(
        SKIP_BUILD
        SRC share/applications share/icons DEST ${PROJECT_NAME}
    )
endif()