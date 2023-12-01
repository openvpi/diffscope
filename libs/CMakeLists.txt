set(QMSETUP_BUILD_DIR "${CMAKE_BINARY_DIR}/out-Libraries-$<LOWER_CASE:${CMAKE_SYSTEM_PROCESSOR}>-$<CONFIG>")

set(QTMEDIATE_INSTALL off)
set(CHORUSKIT_INSTALL off)
set(TALCS_INSTALL off)

add_subdirectory(qtmediate)
add_subdirectory(choruskit)
add_subdirectory(talcs)

# add_dependencies(ChorusKit_UpdateTranslations qtmediate_lupdate_task)
# add_dependencies(ChorusKit_UpdateTranslations choruskit_lupdate_task)

# add_dependencies(ChorusKit_ReleaseTranslations qtmediate_lrelease_task)
# add_dependencies(ChorusKit_ReleaseTranslations choruskit_lrelease_task)