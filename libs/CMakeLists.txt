set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXTERNAL_BUILD_MAIN_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EXTERNAL_BUILD_MAIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${EXTERNAL_BUILD_MAIN_DIR}/lib)
set(CMAKE_BUILD_SHARE_DIR ${EXTERNAL_BUILD_MAIN_DIR}/share)

set(QTMEDIATE_INSTALL off)
set(CHORUSKIT_INSTALL off)
set(TALCS_INSTALL off)

set(QTMEDIATE_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qtmediate/cmake)

ck_add_library_searching_paths(${EXTERNAL_BUILD_MAIN_DIR}/bin)

add_subdirectory(qtmediate)
add_subdirectory(choruskit)
add_subdirectory(talcs)

add_dependencies(ChorusKit_UpdateTranslations qtmediate_lupdate_task)
add_dependencies(ChorusKit_UpdateTranslations choruskit_lupdate_task)

add_dependencies(ChorusKit_ReleaseTranslations qtmediate_lrelease_task)
add_dependencies(ChorusKit_ReleaseTranslations choruskit_lrelease_task)