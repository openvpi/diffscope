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

# Copy files to application share dir
ck_add_shared_files(
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST ${PROJECT_NAME}
)

ck_add_shared_files(
    SKIP_BUILD
    SRC doc/* DEST doc/${PROJECT_NAME}
)

if(WIN32)
    # Add windows icons
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