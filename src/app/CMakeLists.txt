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
elseif(APPLE)
# TODO...
else()
    # Add linux files
    ck_add_shared_files(
        SKIP_BUILD
        SRC share/applications share/icons DEST ${PROJECT_NAME}
    )
endif()