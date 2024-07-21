project(audioplugin VERSION 0.1.0.0)

set(_plugin_name Audio)
set(_plugin_display_name Audio)
option(AUDIO_TEST "Build Audio Plugin Tests" OFF)

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME ${_plugin_name}
    DISPLAY_NAME ${_plugin_display_name}
    COMPAT_VERSION 0.0.0.0
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    MACRO_PREFIX AUDIO
)

if(AUDIO_TEST)
    target_compile_definitions(${PROJECT_NAME} PRIVATE AUDIO_TEST)
endif()

# Configure target
file(GLOB_RECURSE _src *.h *.cpp)
qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Gui Widgets
    QT_INCLUDE_PRIVATE Core Gui Widgets
    LINKS
        ChorusKit::AppCore
        svscraft::Widgets
        coreplugin
        talcs::Core talcs::Device talcs::Format talcs::Midi talcs::Remote talcs::Dspx
    INCLUDE_PRIVATE . internal
)

# Add translation
qm_add_translation(${PROJECT_NAME}_translations
    PREFIX ${_plugin_name}
    TARGETS ${PROJECT_NAME}
    LOCALES zh_CN zh_TW ja_JP
    TS_DIR res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/res/translations
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
        EXCLUDE "${PROJECT_NAME}/internal/.+"
        FORCE
)

ck_sync_include(${PROJECT_NAME}
    DIRECTORY internal
    PREFIX "${PROJECT_NAME}/internal"
    SKIP_INSTALL
    OPTIONS FORCE
)