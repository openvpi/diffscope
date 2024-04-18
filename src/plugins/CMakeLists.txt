qm_import(Translate)
qm_find_qt(LinguistTools)

set(CURRENT_PLUGIN_DESC "${PROJECT_NAME} Application Plugin")

set(_subdirs)
qm_get_subdirs(_subdirs
    EXCLUDE # plugins that will be ignored
)

foreach(_dir IN LISTS _subdirs)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/plugin.json.in")
        add_subdirectory(${_dir})
    endif()
endforeach()