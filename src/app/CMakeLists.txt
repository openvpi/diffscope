ck_configure_application(
    ICO app.ico
    ICNS app.icns
)

qm_configure_target(${PROJECT_NAME}
    SOURCES main.cpp
    QT_LINKS Core Widgets
    LINKS_PRIVATE ChorusKit::Loader ChorusKit::AppCore appshared
    INCLUDE_PRIVATE ${CK_BUILD_INCLUDE_DIR}
)

if(APPLICATION_ENABLE_BREAKPAD)
    find_package(qBreakpad REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE qBreakpad::qBreakpad)
endif()

# Set configration file variables
set(APP_CONFIG_SPLASH_IMAGE "splash.png")
set(APP_CONFIG_PADDING_X 102)
set(APP_CONFIG_PADDING_Y 62)

# Configure configuration file
set(_config_file ${CMAKE_CURRENT_BINARY_DIR}/config.json)
configure_file(config.json.in ${_config_file} @ONLY)

# Deploy docs
ck_add_shared_files(
    SKIP_BUILD
    SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE DEST ${CK_BUILD_DOC_DIR}
)

# Deploy resources
ck_add_shared_files(
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST ${CK_BUILD_DATA_DIR}
)

# Deploy config files
ck_add_shared_files(
    SKIP_BUILD
    SRC conf/${CK_PLATFORM_LOWER}/qt.conf DEST ${CK_BUILD_QT_CONF_DIR}
)

ck_add_shared_files(
    SRC conf/${CK_PLATFORM_LOWER}/qtmediate.json DEST ${CK_BUILD_QT_CONF_DIR}
)

if(WIN32)
    # Deploy windows icons
    ck_add_shared_files(
        SKIP_BUILD
        SRC share/win-icons/ DEST ${CK_BUILD_DATA_DIR}/icons
    )
elseif(APPLE)
# TODO...
else()
    # Add linux files
    ck_add_shared_files(
        SKIP_BUILD
        SRC share/applications share/icons DEST ${CK_BUILD_DATA_DIR}
    )
endif()