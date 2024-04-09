project(iemgr VERSION 0.1.0.0)

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME IEMgr
    COMPAT_VERSION 0.0.0.0
    VENDOR "Sine Striker"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
)

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS
        Core
        Gui
        Widgets
    LINKS
        coreplugin
        opendspx::opendspx
    INCLUDE_PRIVATE
        core
        internal/**
)

# Add translation
qm_import(Translate)
qm_find_qt(LinguistTools)
qm_add_translation(${PROJECT_NAME}_translations
    PREFIX
        IEMgr
    TARGETS
        ${PROJECT_NAME}
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