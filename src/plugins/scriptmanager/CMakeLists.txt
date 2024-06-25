project(scriptmanager VERSION 0.1.0.0)

set(_plugin_name ScriptManager)
set(_plugin_display_name "Script Manager")

option(SCRIPT_MANAGER_TEST "Enable Script Manager Plugin Test" OFF)

# Add target
ck_add_plugin(${PROJECT_NAME}
    NAME ${_plugin_name}
    DISPLAY_NAME ${_plugin_display_name}
    COMPAT_VERSION 0.0.0.0
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    MACRO_PREFIX SCRIPT_MANAGER
)

    # Configure target
file(GLOB_RECURSE _src *.h *.cpp *.qrc)

if (SCRIPT_MANAGER_TEST)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SCRIPT_MANAGER_TEST)
else()
    list(FILTER _src EXCLUDE REGEX tests/)
endif()

qm_configure_target(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Gui Widgets Qml Svg QuickWidgets Test
    QT_INCLUDE_PRIVATE Core Gui Widgets Qml Svg QuickWidgets Test
    LINKS ChorusKit::AppCore svscraft::Widgets stimmt dspxmodel coreplugin
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
    SRC ${CMAKE_CURRENT_SOURCE_DIR}/**.d.ts DEST ./typings
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