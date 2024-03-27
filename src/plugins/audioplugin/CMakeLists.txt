project(audioplugin VERSION 0.1.0.0)

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME Audio
    COMPAT_VERSION 0.0.0.0
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    MACRO_PREFIX AUDIO
)

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Gui Widgets
    QT_INCLUDE_PRIVATE Core Gui Widgets
    LINKS ChorusKit::AppCore svscraft::Widgets coreplugin talcs::Core talcs::Device talcs::Format talcs::Midi talcs::Remote
    INCLUDE_PRIVATE . internal
)

# Add translation
qm_import(Translate)
qm_find_qt(LinguistTools)
qm_add_translation(${PROJECT_NAME}_translations
    PREFIX Audio
    TARGETS ${PROJECT_NAME}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/res/translations
    TS_DEPENDS ChorusKit_UpdateTranslations
    QM_DEPENDS ChorusKit_ReleaseTranslations
)

# Add resources
ck_add_attached_files(${PROJECT_NAME}

    # SRC Res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

# Add install command
ck_sync_include(${PROJECT_NAME}
    OPTIONS EXCLUDE "internal/.+"
)

ck_sync_include(${PROJECT_NAME}
    DIRECTORY internal
    PREFIX "${PROJECT_NAME}/internal"
    SKIP_INSTALL
)