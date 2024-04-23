project(coreplugin VERSION ${APPLICATION_VERSION})

set(_plugin_name Core)
set(_plugin_display_name Core)

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME ${_plugin_name}
    DISPLAY_NAME ${_plugin_display_name}
    COMPAT_VERSION 0.0.0.0
    VENDOR "Sine Striker"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    MACRO_PREFIX CORE
)

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS
        Core
        Gui
        Widgets
    QT_INCLUDE_PRIVATE
        Core
        Gui
        Widgets
    LINKS
        ChorusKit::AppCore
        svscraft::Widgets
        JetBrainsDockingSystem
    INCLUDE_PRIVATE
        core
        windows
        internal/**
)

# Add action
ck_add_action_extension(_action_sources res/core_actions.xml
    DEFINES
        PLUGIN_NAME=${_plugin_display_name}
        APP_NAME=${CK_APPLICATION_NAME}
)
target_sources(${PROJECT_NAME} PRIVATE ${_action_sources})

# Add translation
qm_add_translation(${PROJECT_NAME}_translations
    PREFIX
        ${_plugin_name}
    TARGETS
        ${CK_APPLICATION_NAME} ${PROJECT_NAME}
    LOCALES
        zh_CN zh_TW ja_JP
    TS_DIR
        res/translations
    QM_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/res/translations
    TS_DEPENDS ChorusKit_UpdateTranslations
    QM_DEPENDS ChorusKit_ReleaseTranslations
)

# Add resources
ck_add_attached_files(${PROJECT_NAME}
    SRC res/icons DEST .
    SRC res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/res/translations DEST .
)

# Add install command
ck_sync_include(${PROJECT_NAME}
    OPTIONS
        EXCLUDE "internal/.+"
        FORCE
)

ck_sync_include(${PROJECT_NAME}
    DIRECTORY
        internal
    PREFIX "${PROJECT_NAME}/internal"
    SKIP_INSTALL
)