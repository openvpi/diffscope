project(coreplugin VERSION ${APPLICATION_VERSION})

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME Core
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
        appshared
    INCLUDE_PRIVATE
        core
        windows
        internal/**
)

# Add translation
qm_import(Translate)
qm_find_qt(LinguistTools)
qm_add_translation(${PROJECT_NAME}_translations
    PREFIX
        Core
    TARGETS
        ${PROJECT_NAME}
    LOCALES
        zh_CN zh_HK ja_JP
    TS_DIR
        res/translations
    QM_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/res/translations
    TS_DEPENDS ChorusKit_UpdateTranslations
    QM_DEPENDS ChorusKit_ReleaseTranslations
)

# Add resources
ck_add_attached_files(${PROJECT_NAME}

    # SRC Res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/res/translations DEST .
)

# Add install command
ck_sync_include(${PROJECT_NAME}
    OPTIONS
        EXCLUDE "internal/.+"
)

ck_sync_include(${PROJECT_NAME}
    DIRECTORY
        internal
    PREFIX "${PROJECT_NAME}/internal"
    SKIP_INSTALL
)